/******************************************************************************/
/*!
/*File: Editor.cpp
/*Author: Brady Menendez
*		  Krishna Pillai - Changes to tilemap editor to modify grid height/width
/*Email: brady.m@digipen.edu
*		 krishna.pillai@digipen.edu
/*Date   1/12/2022
/*  Implementation of the Editor Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "EditorUI.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "GameObjectManager.h"
#include "Transform.h"
#include "GLQuad.h"
#include "Texture.h"
#include "Tilemap.h"
#include "GameManager.h"
#include "MemoryManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "Camera.h"
#include "LevelManager.h"
#include "GameObjectFactory.h"
#include "PhysicsWorld.h"
#include "LuaManager.h"
#include "GameDefs.h"
#include "Collider.h"

std::vector<const char*> obj_def_list = {
	"enemy_obj",
	"fire_projectile",
	"ice_ball",
	"obj1",
	"obj2",
	"obj3",
	"Tilemap",
	"Tilemap1"
};


/* Initializes imgui */
void Editor::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(p_graphics_manager->p_sdl_window,
								 p_graphics_manager->p_gl_context);

	std::string glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());


	for (auto& obj_def : obj_def_list)
	{
		std::string obj_def_file = ".\\Obj_defs\\" + std::string(obj_def) + ".json";
		std::ifstream obj_def_data(obj_def_file);

		if (obj_def_data.good())
		{
			std::stringstream buffer;
			buffer << obj_def_data.rdbuf();
			obj_def_json_strings.insert(std::pair<std::string, std::string>(obj_def, buffer.str()));
		}
	}

	/*crosshair = new GameObject("crosshair");
	crosshair->AddComponent(new Transform());
	crosshair->AddComponent(new GLQuad());*/


}

/* Sets up new imgui frame */
void Editor::NewFrame() const {
	//ImGui new frame setup
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Editor::EditorCameraControls()
{
	// test camera movement lines
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_UP))
		p_camera->ProcessKeyboardInput(CameraMovement::CAM_UP);
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_LEFT))
		p_camera->ProcessKeyboardInput(CameraMovement::CAM_LEFT);
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_DOWN))
		p_camera->ProcessKeyboardInput(CameraMovement::CAM_DOWN);
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_RIGHT))
		p_camera->ProcessKeyboardInput(CameraMovement::CAM_RIGHT);
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_Z))
		p_camera->ProcessKeyboardInput(CameraMovement::CAM_FORWARD);
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_X))
		p_camera->ProcessKeyboardInput(CameraMovement::CAM_BACKWARD);
}

/* Debugging window implementation */
void Editor::DebuggerWindow() {
	ImGui::Begin("Debug Info");

	ImGui::Text("FPS: %s", std::to_string(last_frame_fps).c_str());
	ImGui::Text("Memory Consumption (Kb): %s",
		std::to_string(g_memory_manager.GetMemoryUsedKBytes()).c_str());

	int x, y;

	SDL_GetMouseState(&x, &y);

	mouse_worldpos = p_camera->ScreenToWorld(x, y);

	std::string mouse_coord = "Mouse coord: (" + std::to_string(x) + ", " + std::to_string(y) + ")";
	std::string mouse_pos = "Mouse Pos: (" + std::to_string(mouse_worldpos.x) + ", " + std::to_string(mouse_worldpos.y) + ")";

	std::string camera_pos = "Camera Position : (" + 
		std::to_string(p_camera->position.x) + ", " + 
		std::to_string(p_camera->position.y) + ", " + 
		std::to_string(p_camera->position.z) + ")";

	ImGui::Text(mouse_pos.c_str());
	ImGui::Text(mouse_coord.c_str());
	ImGui::Text(camera_pos.c_str());

	ImGui::InputInt("Target Level", &level_num);

	if (ImGui::Button("Load"))
		p_level_manager->LoadLevel(level_num);

	ImGui::Text("Current Level: %d", p_level_manager->current_level);
	if (ImGui::Button("Save"))
		p_level_manager->SaveLevel();

	ImGui::End();
}

/* GameObjectList window implementation */
void Editor::GameObjectWindow() {
	//ImGui::SetNextWindowSize(ImVec2(500, 440));
	ImGui::Begin("GameObjectList");

	std::vector<const char*> obj_names;
	int objlist_size = p_game_obj_manager->game_object_list.size();

	for (int i = 0; i < objlist_size; ++i)
		obj_names.push_back(p_game_obj_manager->game_object_list[i]->GetNameRef().c_str());

	

	// Left Item
	{
		ImGui::BeginChild("left pane", ImVec2(250, 0), true);
		ImGui::ListBox("List", &selected, obj_names.data(), objlist_size);

		ImGui::EndChild();
	}
	ImGui::SameLine();

	// Right Item
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		ImGui::Text("Selected Object: %s", obj_names[selected]);
		ImGui::Separator();

		GameObject* curr_obj = p_game_obj_manager->game_object_list[selected];
		Component* comp_transform = p_game_obj_manager->game_object_list[selected]->HasComponent("TRANSFORM");
		Component* comp_glquad = p_game_obj_manager->game_object_list[selected]->HasComponent("GLQUAD");
		Component* comp_tilemap = p_game_obj_manager->game_object_list[selected]->HasComponent("TILEMAP");
		Component* collider = p_game_obj_manager->game_object_list[selected]->HasComponent("COLLIDER");

		// Component tabs
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			// transform component tab
			if (comp_transform)
			{
				Transform* obj_transform = static_cast<Transform*>(comp_transform);
				if (ImGui::BeginTabItem("Transform"))
				{
					// position value handling
					ImGui::Text("Position: ");

					static float posX = obj_transform->GetPosition().x;
					static float posY = obj_transform->GetPosition().y;
					static float posZ = obj_transform->GetPosition().z;

					if (current_index != selected)
					{
						posX = obj_transform->GetPosition().x;
						posY = obj_transform->GetPosition().y;
						posZ = obj_transform->GetPosition().z;
					}

					ImGui::DragFloat("Pos.X", &posX, 0.2f, -100000.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Pos.Y", &posY, 0.2f, -100000.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Pos.Z", &posZ, 0.2f, -100000.0f, 100000.0f, "%.3f");

					// save new values
					glm::vec4 new_pos{};
					new_pos.x = posX;
					new_pos.y = posY;
					new_pos.z = posZ;
					obj_transform->SetPosition(new_pos);

					ImGui::Separator();
					// scale value handling
					ImGui::Text("Scale: ");

					static float scaleX = obj_transform->GetScaleX();
					static float scaleY = obj_transform->GetScaleY();

					if (current_index != selected)
					{
						scaleX = obj_transform->GetScaleX();
						scaleY = obj_transform->GetScaleY();
					}

					ImGui::DragFloat("Sca.X", &scaleX, 0.2f, -100000.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Sca.Y", &scaleY, 0.2f, -100000.0f, 100000.0f, "%.3f");

					// save new values
					obj_transform->SetScale(scaleX, scaleY);

					ImGui::Separator();
					// scale value handling
					ImGui::Text("Rotation: ");

					static float rot = obj_transform->GetRotation();

					if (current_index != selected)
					{
						rot = obj_transform->GetRotation();
					}

					ImGui::DragFloat("Rot", &rot, 0.2f, 1.0f, 100000.0f, "%.3f");

					// save new values
					obj_transform->SetRotation(rot);

					ImGui::Separator();

					if (ImGui::Button("Revert"))
					{
						// revert to previous values
						auto obj_def = p_level_manager->curr_obj_map[curr_obj->GetName()];
						if (obj_def.contains("starting_position")) {
							auto saved_pos = obj_def["starting_position"]
								.get<std::vector<float>>();

							posX = saved_pos[0];
							posY = saved_pos[1];
							posZ = saved_pos[2];

							obj_transform->SetPosition(glm::vec4(saved_pos[0], saved_pos[1], saved_pos[2], saved_pos[3]));
						}
						
						if (obj_def.contains("starting_scale")) {
							auto saved_scale = obj_def["starting_scale"]
								.get<std::vector<float>>();

							scaleX = saved_scale[0];
							scaleY = saved_scale[1];

							obj_transform->SetScale(saved_scale[0], saved_scale[1]);
						}

						obj_transform->Update();
					}
					ImGui::SameLine();
					if (ImGui::Button("Save"))
					{
						// save new values
						std::vector<float> f_vector = { posX,
														posY,
														posZ,
														obj_transform->GetPosCoord(3) };

						p_level_manager->curr_obj_map[curr_obj->GetName()]["starting_position"]
							= json(f_vector);

						std::vector<float> scale_vector = { scaleX, scaleY };
						p_level_manager->curr_obj_map[curr_obj->GetName()]["starting_scale"]
							= json(scale_vector);
						p_level_manager->SaveSingle();

						obj_transform->Update();
					}

					ImGui::EndTabItem();
				}
			}

			// GLQuad Component Tab
			if (comp_glquad)
			{
				GLQuad* obj_glquad = static_cast<GLQuad*>(comp_glquad);
				if (ImGui::BeginTabItem("Rendering"))
				{
					ImGui::Text(("Texture Name: " + obj_glquad->GetTexture()->name).c_str());
					ImGui::Checkbox("Debug Draw", &p_game_manager->debug_mode);
					ImGui::EndTabItem();
				}

			}

			// Tilemap
			if (comp_tilemap)
			{
				Tilemap* obj_glquad = static_cast<Tilemap*>(comp_tilemap);
				if (ImGui::BeginTabItem("Tilemap"))
				{
					if (ImGui::Button("Select Tilemap"))
					{
						current_tilemap = curr_obj;
					}
					ImGui::EndTabItem();
				}

			}

			// Collider
			if (collider)
			{
				Collider* collider_comp = static_cast<Collider*>(collider);
				if (ImGui::BeginTabItem("Collider"))
				{
					// position value handling
					ImGui::Text("Collider offset: ");
					static float posX = collider_comp->GetPositionOffset().x;
					static float posY = collider_comp->GetPositionOffset().y;

					if (current_index != selected)
					{
						static float posX = collider_comp->GetPositionOffset().x;
						static float posY = collider_comp->GetPositionOffset().y;
					}

					ImGui::DragFloat("Pos.X", &posX, 0.2f, -100000.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Pos.Y", &posY, 0.2f, -100000.0f, 100000.0f, "%.3f");
					ImGui::SameLine;

					ImGui::Separator();
					// scale value handling
					ImGui::Text("Collider dimensions: ");

					static float width = collider_comp->GetPositionOffset().z;
					static float height = collider_comp->GetPositionOffset().w;

					if (current_index != selected)
					{
						width = collider_comp->GetPositionOffset().z;
						height = collider_comp->GetPositionOffset().w;
					}

					ImGui::DragFloat("Width ", &width, 0.2f, 1.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Height", &height, 0.2f, 1.0f, 100000.0f, "%.3f");

					ImGui::Separator();
					
					// save new values
					glm::vec4 new_pos{};
					new_pos.x = posX;
					new_pos.y = posY;
					new_pos.z = width;
					new_pos.w = height;
					collider_comp->SetPositionOffset(new_pos);

					if (ImGui::Button("Revert"))
					{
						// revert to previous values

						std::string obj_def = p_level_manager->curr_obj_map[curr_obj->GetName()]["obj_def"];

						std::string obj_def_file = ".\\Obj_defs\\" + obj_def + ".json";
						std::ifstream obj_def_data(obj_def_file);

						json json_object;
						obj_def_data >> json_object;
						obj_def_data.close();

						auto pos_vec = json_object["COMPONENTS"]["COLLIDER"]["collider_offset_params"].get<std::vector<float>>();

						collider_comp->SetPositionOffset(glm::vec4(pos_vec[0], pos_vec[1], pos_vec[2], pos_vec[3]));

						collider->Update();
					}
					ImGui::SameLine();
					if (ImGui::Button("Save"))
					{
						std::string obj_def = p_level_manager->curr_obj_map[curr_obj->GetName()]["obj_def"];

						std::string obj_def_file = ".\\Obj_defs\\" + obj_def + ".json";
						std::ifstream obj_def_data(obj_def_file);

						json json_object;
						obj_def_data >> json_object;
						obj_def_data.close();

						std::vector<float> out_vec;
						out_vec.push_back(new_pos.x);
						out_vec.push_back(new_pos.y);
						out_vec.push_back(new_pos.z);
						out_vec.push_back(new_pos.w);

						json_object["COMPONENTS"]["COLLIDER"]["collider_offset_params"] = out_vec;

						std::ofstream o(obj_def_file);
						o << std::setw(4) << json_object << std::endl;
						o.close();

						collider->Update();
					}

					ImGui::EndTabItem();
				}

			}

			// JSON obj_def Tab
			if (ImGui::BeginTabItem("OBJ_DEF"))
			{
				ImGui::Text(curr_obj->obj_def.c_str());
				ImGui::Text(obj_def_json_strings[curr_obj->obj_def].c_str());
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();

		if (current_index != selected)
			current_index = selected;

		ImGui::EndGroup();
	}
	ImGui::End();
}

/* TileEditor window implementation */
void Editor::TileEditorWindow() {
	ImGui::Begin("Tile Editor");

	Transform* target_transform;
	Tilemap* target_tilemap;

	if (current_tilemap == nullptr)
	{
		ImGui::Text("To start, click on SELECT TILEMAP on an object's Tilemap tab");
		ImGui::End();
		return;
	}
	

	target_transform = static_cast<Transform*>
				(current_tilemap->HasComponent("TRANSFORM"));
	target_tilemap = static_cast<Tilemap*>
				(current_tilemap->HasComponent("TILEMAP"));

	std::string text = "CURRENT TILEMAP: " + current_tilemap->GetName();
	int index_x = -1;
	int index_y = -1;
	int texcoord_first = -1;
	GLuint texture_int = 0;
	ImGui::Text(text.c_str());

	//IMGUI UI for changing grid width
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Grid width :");
	ImGui::SameLine();

	// Arrow buttons with Repeater
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::PushButtonRepeat(false);
	if (ImGui::ArrowButton("##width_decrement", ImGuiDir_Left)) {
		target_tilemap->DecrementGridWidth();
		target_tilemap->RegenTexCoords();
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##width_increment", ImGuiDir_Right)) {
		target_tilemap->IncrementGridWidth();
		target_tilemap->RegenTexCoords();
	}
	ImGui::PopButtonRepeat();
	ImGui::SameLine();
	std::string grid_width = "GRID_WIDTH : " + std::to_string(target_tilemap->GetGridWidth());
	ImGui::Text(grid_width.c_str());

	//IMGUI UI for changing grid height
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Grid height :");
	ImGui::SameLine();

	// Arrow buttons with Repeater
	ImGui::PushButtonRepeat(false);
	if (ImGui::ArrowButton("##height_decrement", ImGuiDir_Left)) {
		target_tilemap->DecrementGridHeight();
		target_tilemap->RegenTexCoords();
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##height_increment", ImGuiDir_Right)) {
		target_tilemap->IncrementGridHeight();
		target_tilemap->RegenTexCoords();
	}
	ImGui::PopButtonRepeat();
	ImGui::SameLine();
	std::string grid_height = " GRID_HEIGHT : " + std::to_string(target_tilemap->GetGridHeight());
	ImGui::Text(grid_height.c_str());

	if (target_transform && target_tilemap)
	{
		glm::vec3 topleft = target_transform->GetPosition();
		glm::vec3 bottomright = topleft + glm::vec3(target_tilemap->GetDimensions().z,
			target_tilemap->GetDimensions().w, 0.0f);

		glm::vec3 cam_pos = p_camera->position;

		if (topleft.x <= cam_pos.x && cam_pos.x <= bottomright.x &&
			topleft.y <= cam_pos.y && cam_pos.y <= bottomright.y)
		{
		
			glm::vec2 proj_pos = glm::vec2(cam_pos.x - target_transform->GetPosition().x, 
				cam_pos.y - target_transform->GetPosition().y);

			index_x = static_cast<int>(proj_pos.x) / target_tilemap->GetTileWidth();
			index_y = static_cast<int>(proj_pos.y) / target_tilemap->GetTileHeight();

			texcoord_first = (target_tilemap->GetGridWidth() * index_y + index_x) * 8;

			std::string coords = "TARGET:  X: " + std::to_string(index_x) + ", Y: " + std::to_string(index_y);
			ImGui::Text(coords.c_str());

			ImVec2 uv0 = ImVec2(target_tilemap->GetTexCoords()[texcoord_first],
				target_tilemap->GetTexCoords()[texcoord_first + 1]);

			ImVec2 uv1 = ImVec2(target_tilemap->GetTexCoords()[texcoord_first + 4],
				target_tilemap->GetTexCoords()[texcoord_first + 5]);

			/*ImVec2 uv0 = ImVec2(0, 0);
			ImVec2 uv1 = ImVec2(0.8, 0.8);*/

			ImGui::Image((void*)(intptr_t)target_tilemap->GetTexture()->texture_id, ImVec2(100, 100), uv0, uv1);

			ImGui::Separator();

			static int currTile = 0;

			ImGui::Text("SELECT A NEW TEXTURE, THEN CLICK IT TO CHANGE: ");


			ImGui::SliderInt("TILE SELECT", &currTile, 0, 107);

			int button_x = currTile % 12;
			int button_y = currTile / 12;

			ImVec2 uv2 = ImVec2(float(button_x) / 12.0f,
				float(button_y) / 9.0f);

			ImVec2 uv3 = ImVec2(float(button_x + 1) / 12.0f,
				float(button_y + 1) / 9.0f);


			// (4, 5), W: 12, H: 9
			// 64 -> 5 * 12 + 4

			// 4/12    ,  5/9       top left
			// 5/12    ,  6/9       bot right

			
			if (ImGui::ImageButton((void*)(intptr_t)target_tilemap->GetTexture()->texture_id, ImVec2(100, 100), uv2, uv3))
			{

				target_tilemap->GetTileIndexMap()[index_y][index_x][0] = button_x;
				target_tilemap->GetTileIndexMap()[index_y][index_x][1] = button_y;

				target_tilemap->RegenTexCoords();

				std::cout << "click\n";
			}

			if (ImGui::Button("Save new tilemap as JSON"))
			{
				
				std::string obj_def = p_level_manager->curr_obj_map[current_tilemap->GetName()]["obj_def"];

				std::string obj_def_file = ".\\Obj_defs\\" + obj_def + ".json";
				std::ifstream obj_def_data(obj_def_file);

				json json_object;
				obj_def_data >> json_object;
				obj_def_data.close();

				json_object["COMPONENTS"]["TILEMAP"]["tile_index_map"] = json(target_tilemap->GetTileIndexMap());
				//target_tilemap->GetObjMap()["tile_index_map"] = 
				
				std::ofstream o(obj_def_file);
				o << std::setw(4) << json_object << std::endl;
				o.close();
			}

		}
	}

	ImGui::End();

	int cursor_window_width = 5;
	int cursor_window_height = 5;

	float window_center_x = WINDOW_WIDTH / 2.0f;
	float window_center_y = WINDOW_HEIGHT / 2.0f;

	ImGui::SetNextWindowPos(ImVec2(window_center_x, window_center_y));
	ImGui::SetNextWindowSize(ImVec2(cursor_window_width, cursor_window_height));
	ImGui::SetNextWindowCollapsed(false);

	ImGui::Begin(" ");
	ImGui::End();
}


/* Main imgui loop: GameObjectList Window */
void Editor::Render() {
	
	DebuggerWindow();
	GameObjectWindow();
	TileEditorWindow();
	EditorCameraControls();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/* Shuts down imgui environment */
void Editor::Cleanup() const {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
