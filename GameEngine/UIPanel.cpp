/******************************************************************************/
/*!
/*File   UIPanel.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  UIPanel implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "UIPanel.h"
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

UIPanel::UIPanel() : Component("UIPANEL"), pixel_position(0.0),
					pixel_dimensions(0.0), quad_pixel_dimensions(0.0),
					p_texture(nullptr), grid_width(0), grid_height(0),
					color_coords(), tex_coords(), vertex_coords(),
					vao_id(0), visible(true), tile_map_index(),
					vertex_buffer_id(), color_buffer_id(), 
					texture_buffer_id(), texture_mode(1), 
					p_owner_transform(nullptr), scale(1.0f) {}


/*Serialize the UIPanel from a json object
* Expects a dict with the following key:values
* pixel_position : [float, float]
* quad_pixel_dimensions : [float, float]
* texture_name : string
* tile_map_index : [ [int, int] * number_of_tiles ]
* Returns: void
*/
void UIPanel::Serialize(json json_object) {

	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	p_texture = p_resource_manager->get_texture(texture_name);

	//The tile map index is a 2D array with each element containing [tex_offset_x, tex_offset_y]
	tile_map_index =
		json_object["tile_map_index"].get<
		std::vector<std::vector<std::vector<unsigned int>>>>();

	quad_pixel_dimensions[0] = json_object["quad_pixel_dimensions"].get<std::vector<int>>()[0];
	quad_pixel_dimensions[1] = json_object["quad_pixel_dimensions"].get<std::vector<int>>()[1];

	grid_width = tile_map_index[0].size(); //Number of columns in the tile_map_index
	grid_height = tile_map_index.size(); //Number of rows in the tile_map_index

	pixel_dimensions[0] = grid_width * quad_pixel_dimensions[0];
	pixel_dimensions[1] = grid_height * quad_pixel_dimensions[1];

	pixel_position[0] = json_object["pixel_position"].get<std::vector<int>>()[0];
	pixel_position[1] = json_object["pixel_position"].get<std::vector<int>>()[1];

	scale.x = json_object["scale"].get<std::vector<float>>()[0];
	scale.y = json_object["scale"].get<std::vector<float>>()[1];

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

	vao_id = p_graphics_manager->GenerateDynamicQuadVAO(vertex_buffer_id,
		color_buffer_id, texture_buffer_id, grid_width * grid_height);
}

void UIPanel::Link() {
	p_owner_transform =
		static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_transform->SetPosition(glm::vec4(pixel_position.x, pixel_position.y, 0, 1));
	p_owner_transform->SetScale(scale.x, scale.y);
}

void UIPanel::GenerateTilemapVertices() {
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

void UIPanel::GenerateTilemapTextureCoords() {
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
}

void UIPanel::Update() {

}

void UIPanel::Draw(ShaderProgram* p_shader_program) {
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

	p_graphics_manager->SetDynamicBufferData(vao_id, texture_buffer_id, &tex_coords[0],
		sizeof(float) * tex_coords.size());

	if (p_game_manager->GetDegugMode())
		texture_mode = 0;
	else
		texture_mode = 1;

	p_graphics_manager->SetUniformInt(texture_mode, "mode");

	p_texture->Bind(2, p_graphics_manager->GetActiveShader()->program_id,
		"texture_map");

	p_graphics_manager->SetUniformInt(0, "text");

	p_graphics_manager->DrawQuad(vao_id, grid_width * grid_height, false);

	p_graphics_manager->SetDepthTestOn();
	p_graphics_manager->SetActiveShader("final");
}