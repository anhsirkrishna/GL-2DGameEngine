/******************************************************************************/
/*!
/*File   Button.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  Button implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Button.h"
#include "ResourceManager.h"
#include "Util.h"
#include "Texture.h"
#include "GraphicsManager.h"
#include "Behavior.h"
#include "InputManager.h"
#include "GameManager.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "GameObject.h"

#include <SDL.h>

Button::Button() : Component("BUTTON"), pixel_position(0.0), 
				pixel_dimensions(0.0), quad_pixel_dimensions(0.0),
				p_texture(nullptr), grid_width(0), grid_height(0),
				color_coords(), tex_coords(), vertex_coords(),
				button_action(nullptr), vao_id(0), visible(true),
				button_down(false), tile_map_index(), 
				clicked_tile_map_index(), vertex_buffer_id(),
				color_buffer_id(), texture_buffer_id(), p_behavior(nullptr),
				texture_mode(1), p_owner_transform(nullptr), scale(1.0) {}

Button::~Button() {
	delete p_behavior;
}

/*Serialize the button from a json object
* Expects a dict with the following key:values
* pixel_position : [float, float]
* quad_pixel_dimensions : [float, float]
* texture_name : string
* tile_map_index : [ [int, int] * number_of_tiles ]
* clicked_tile_map_index : [ [int, int] * number_of_tiles ]
* action_script: string
* Returns: void
*/
void Button::Serialize(json json_object) {

	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	p_texture = p_resource_manager->get_texture(texture_name);

	//The tile map index is a 2D array with each element containing [tex_offset_x, tex_offset_y]
	tile_map_index = 
		json_object["tile_map_index"].get<
		std::vector<std::vector<std::vector<unsigned int>>>>();
	clicked_tile_map_index = 
		json_object["clicked_tile_map_index"].get<
		std::vector<std::vector<std::vector<unsigned int>>>>();
	quad_pixel_dimensions[0] = json_object["quad_pixel_dimensions"].get<std::vector<int>>()[0];
	quad_pixel_dimensions[1] = json_object["quad_pixel_dimensions"].get<std::vector<int>>()[0];
	
	grid_width = tile_map_index[0].size(); //Number of columns in the tile_map_index
	grid_height = tile_map_index.size(); //Number of rows in the tile_map_index

	pixel_position[0] = json_object["pixel_position"].get<std::vector<int>>()[0];
	pixel_position[1] = json_object["pixel_position"].get<std::vector<int>>()[1];

	scale.x = json_object["scale"].get<std::vector<int>>()[0];
	scale.y = json_object["scale"].get<std::vector<int>>()[1];

	pixel_dimensions[0] = grid_width * quad_pixel_dimensions[0] * scale.x;
	pixel_dimensions[1] = grid_height * quad_pixel_dimensions[1] * scale.y;

	GenerateTilemapVertices();

	for (unsigned int i = 0; i < grid_width * grid_height; ++i) {
		//Green color for all tilemaps
		//Top left vertex
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);

		//Top right vertex
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);

		//Bottom left vertex
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);

		//Bottom right vertex
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);
		color_coords.push_back(0.0f);
		color_coords.push_back(1.0f);
	}

	GenerateTilemapTextureCoords();

	//Convert coords from image space to 0..1
	ConvertTextureCoords(tex_coords, p_texture->width, p_texture->height);
	ConvertTextureCoords(clicked_tex_coords, p_texture->width, p_texture->height);

	vao_id = p_graphics_manager->GenerateDynamicQuadVAO(vertex_buffer_id, 
		color_buffer_id, texture_buffer_id, grid_width * grid_height);

	p_behavior = new Behavior();
	p_behavior->SetScriptName(json_object["action_script"].get<std::string>());
}

void Button::Link() {
	p_behavior->SetOwner(GetOwner());
	p_behavior->LoadScript();

	p_owner_transform =
		static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_transform->SetPosition(glm::vec4(pixel_position.x, pixel_position.y, 0, 1));
	p_owner_transform->SetScale(scale.x, scale.y);
}

void Button::GenerateTilemapVertices() {
	vertex_coords.clear();
	float tile_width = quad_pixel_dimensions[0];
	float tile_height = quad_pixel_dimensions[1];

	for (unsigned int i = 0; i < grid_height; ++i) {
		for (unsigned int j = 0; j < grid_width; ++j) {
			//4 vertices per quad
			vertex_coords.push_back((j * tile_width));
			vertex_coords.push_back((i * tile_height));
			vertex_coords.push_back(0.0f);

			vertex_coords.push_back((j * tile_width));
			vertex_coords.push_back((i * tile_height) + tile_height);
			vertex_coords.push_back(0.0f);

			vertex_coords.push_back((j * tile_width) + tile_width);
			vertex_coords.push_back((i * tile_height) + tile_height);
			vertex_coords.push_back(0.0f);

			vertex_coords.push_back((j * tile_width) + tile_width);
			vertex_coords.push_back((i * tile_height));
			vertex_coords.push_back(0.0f);
		}
	}
}

void Button::GenerateTilemapTextureCoords() {
	float tile_width = quad_pixel_dimensions[0];
	float tile_height = quad_pixel_dimensions[1];

	for (unsigned int i = 0; i < grid_height; ++i) {
		for (unsigned int j = 0; j < grid_width; ++j) {
			//4 vertices per quad
			//Top left vertex
			tex_coords.push_back((tile_map_index[i][j][0] * tile_width));
			tex_coords.push_back((tile_map_index[i][j][1] * tile_height));

			//Bottom left vertex
			tex_coords.push_back((tile_map_index[i][j][0] * tile_width));
			tex_coords.push_back((tile_map_index[i][j][1] * tile_height) + tile_height);

			//Bottom right vertex
			tex_coords.push_back((tile_map_index[i][j][0] * tile_width) + tile_width);
			tex_coords.push_back((tile_map_index[i][j][1] * tile_height) + tile_height);

			//Top right vertex
			tex_coords.push_back((tile_map_index[i][j][0] * tile_width) + tile_width);
			tex_coords.push_back((tile_map_index[i][j][1] * tile_height));
		}
	}

	for (unsigned int i = 0; i < grid_height; ++i) {
		for (unsigned int j = 0; j < grid_width; ++j) {
			//4 vertices per quad
			//Top left vertex
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][0] * tile_width));
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][1] * tile_height));

			//Bottom left vertex
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][0] * tile_width));
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][1] * tile_height) + tile_height);

			//Bottom right vertex
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][0] * tile_width) + tile_width);
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][1] * tile_height) + tile_height);

			//Top right vertex
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][0] * tile_width) + tile_width);
			clicked_tex_coords.push_back((clicked_tile_map_index[i][j][1] * tile_height));
		}
	}
}

void Button::Update() {
	if (p_input_manager->isMouseTriggered(SDL_BUTTON_LMASK)) {
		if (p_input_manager->mouse_x > pixel_position.x &&
			p_input_manager->mouse_x < pixel_position.x + pixel_dimensions.x &&
			p_input_manager->mouse_y > pixel_position.y &&
			p_input_manager->mouse_y < pixel_position.y + pixel_dimensions.y) {
			button_down = true;
		}
	}

	if (p_input_manager->isMouseReleased(SDL_BUTTON_LMASK)) {
		if (button_down == true) {
			button_down = false;
			Click();
		}
	}
}

void Button::Click() {
	if (button_action != nullptr)
		button_action();
	p_behavior->Update();
}

void Button::SetButtonAction(void (*_button_action)()) {
	button_action = _button_action;
}

void Button::Draw(ShaderProgram *p_shader_program) {
	p_graphics_manager->SetActiveShader("ui");
	p_graphics_manager->SetDepthTestOff();
	p_graphics_manager->SetOrthographicMatrix();

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetTranslateMatrix(), "translateMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetRotateMatrix(), "rotateMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetScaleMatrix(), "scaleMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetPreRotateMatrix(), "preRotateMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetPostRotateMatrix(), "postRotateMatrix");

	p_graphics_manager->SetDynamicBufferData(vao_id, vertex_buffer_id, &vertex_coords[0],
		sizeof(float) * vertex_coords.size());

	p_graphics_manager->SetDynamicBufferData(vao_id, color_buffer_id, &color_coords[0],
		sizeof(float) * color_coords.size());

	if (!button_down)
		p_graphics_manager->SetDynamicBufferData(vao_id, texture_buffer_id, &tex_coords[0],
			sizeof(float) * tex_coords.size());
	else
		p_graphics_manager->SetDynamicBufferData(vao_id, texture_buffer_id, &clicked_tex_coords[0],
			sizeof(float) * clicked_tex_coords.size());


	if (p_game_manager->GetDegugMode())
		texture_mode = 0;
	else
		texture_mode = 1;

	p_graphics_manager->SetUniformInt(texture_mode, "mode");

	p_texture->Bind(2, p_graphics_manager->GetActiveShader()->program_id,
		"texture_map");

	p_graphics_manager->DrawQuad(vao_id, grid_width * grid_height, false);

	p_graphics_manager->SetDepthTestOn();
	p_graphics_manager->SetActiveShader("final");
}