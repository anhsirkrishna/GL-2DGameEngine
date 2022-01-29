/******************************************************************************/
/*!
/*File: Editor.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   1/12/2022
/*  Implementation of the Editor Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "EditorUI.h"
#include <string>
#include <vector>
#include <iostream>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include "GameObjectManager.h"
#include "Transform.h"
#include "GLQuad.h"
#include "Texture.h"

/* Initializes imgui */
void Editor::Init(SDL_Window* window, SDL_GLContext context) const {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, context);

	std::string glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

/* Sets up new imgui frame */
void Editor::NewFrame() const {
	//ImGui new frame setup
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

/* Main imgui loop: GameObjectList Window */
void Editor::Render() {
	//ImGui::SetNextWindowSize(ImVec2(500, 440));
	ImGui::Begin("GameObjectList");

	std::vector<const char*> obj_names;
	int objlist_size = p_game_obj_manager->game_object_list.size();

	for (int i = 0; i < objlist_size; ++i)
		obj_names.push_back(p_game_obj_manager->game_object_list[i]->GetNameRef().c_str());

	static int selected = 0;
	static int current_index = 0;

	// Left Item
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
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

		Component* comp_transform = p_game_obj_manager->game_object_list[selected]->HasComponent("TRANSFORM");
		Component* comp_glquad = p_game_obj_manager->game_object_list[selected]->HasComponent("GLQuad");

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

					if (current_index != selected)
					{
						posX = obj_transform->GetPosition().x;
						posY = obj_transform->GetPosition().y;
					}

					ImGui::DragFloat("Pos.X", &posX, 0.2f, -100000.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Pos.Y", &posY, 0.2f, -100000.0f, 100000.0f, "%.3f");

					// save new values
					glm::vec4 new_pos{};
					new_pos.x = posX;
					new_pos.y = posY;
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

					ImGui::DragFloat("Sca.X", &scaleX, 0.2f, 1.0f, 100000.0f, "%.3f");
					ImGui::SameLine;
					ImGui::DragFloat("Sca.Y", &scaleY, 0.2f, 1.0f, 100000.0f, "%.3f");

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

					if (ImGui::Button("Revert"))
					{
						// revert to previous values
					}
					ImGui::SameLine();
					if (ImGui::Button("Save"))
					{
						// save new values
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

					ImGui::EndTabItem();
				}

			}

			// Sample Details Tab
			if (ImGui::BeginTabItem("Details"))
			{
				ImGui::Text("ID: 0123456789");
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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/* Shuts down imgui environment */
void Editor::Cleanup() const {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
