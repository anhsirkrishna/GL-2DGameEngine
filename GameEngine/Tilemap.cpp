/******************************************************************************/
/*!
/*File   Tilemap.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Tilemap Component implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Tilemap.h"
#include "GLQuad.h"
#include "Transform.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Util.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "GraphicsManager.h"
#include "GameManager.h"

Tilemap::Tilemap() : Component("TILEMAP"), p_texture(NULL),
					 p_owner_transform(NULL), grid_width(0),
					 grid_height(0), tile_width(0), tile_height(0),
					 dimensions(), vao_id(0), texture_mode(1), 
					 brightness(1.0) {
}

void Tilemap::Serialize(json json_object) {

	obj_map = json_object;

	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	p_texture = p_resource_manager->get_texture(texture_name);

	//The tile index map is a 2D array with each element containing [tex_offset_x, tex_offset_y]
	tile_index_map = json_object["tile_index_map"].get<std::vector<std::vector<std::vector<int>>>>();
	tile_width = json_object["tile_width"].get<int>();
	tile_height = json_object["tile_height"].get<int>();
	grid_width = tile_index_map[0].size(); //Number of columns in the tile_index_map
	grid_height = tile_index_map.size(); //Number of rows in the tile_index_map

	dimensions.z = grid_width * tile_width;
	dimensions.w = grid_height * tile_height;

	GenerateTilemapVertices(vertices);

	for (unsigned int i = 0; i < grid_width * grid_height; ++i) {
		//Green color for all tilemaps
		//Top left vertex
		colors.push_back(0.0f);
		colors.push_back(1.0f);
		colors.push_back(0.0f);
		colors.push_back(1.0f);

		//Top right vertex
		colors.push_back(0.0f);
		colors.push_back(1.0f);
		colors.push_back(0.0f);
		colors.push_back(1.0f);

		//Bottom left vertex
		colors.push_back(0.0f);
		colors.push_back(1.0f);
		colors.push_back(0.0f);
		colors.push_back(1.0f);

		//Bottom right vertex
		colors.push_back(0.0f);
		colors.push_back(1.0f);
		colors.push_back(0.0f);
		colors.push_back(1.0f);
	}

	GenerateTilemapTextureCoords(tex_coords);

	//Convert coords from image space to 0..1
	ConvertTextureCoords(tex_coords, p_texture->width, p_texture->height);

	vao_id = p_graphics_manager->GenerateQuadVAO(
		&vertices[0], &colors[0], &tex_coords[0], grid_width*grid_height);
}

void Tilemap::Link() {
	p_owner_transform =
		static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	dimensions.x = p_owner_transform->GetPosition().x;
	dimensions.y = p_owner_transform->GetPosition().y;
}

void Tilemap::Draw(ShaderProgram* p_program) {
	p_graphics_manager->SetProjectionMatrix();
	p_graphics_manager->SetViewMatrix();

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

	p_texture->Bind(2, p_graphics_manager->GetActiveShader()->program_id,
		"texture_map");

	glm::vec2 converted_tex_offset(0.0f);
	p_graphics_manager->SetUniformVec2(converted_tex_offset, "tex_offset");

	if (p_game_manager->GetDegugMode())
		SetTextureMode(0);
	else
		SetTextureMode(1);

	p_graphics_manager->SetUniformVec3(brightness, "brightness");

	p_graphics_manager->SetUniformInt(texture_mode, "mode");

	p_graphics_manager->DrawQuad(vao_id, grid_width*grid_height);
}

void Tilemap::GenerateTilemapVertices(std::vector<float> &vertices) {
	vertices.clear();

	for (unsigned int i = 0; i < grid_height; ++i) {
		for (unsigned int j = 0; j < grid_width; ++j) {
			//4 vertices per quad
			vertices.push_back((j * tile_width));
			vertices.push_back((i * tile_height));
			vertices.push_back(0.0f);

			vertices.push_back((j * tile_width));
			vertices.push_back((i * tile_height) + tile_height);
			vertices.push_back(0.0f);

			vertices.push_back((j * tile_width) + tile_width);
			vertices.push_back((i * tile_height) + tile_height);
			vertices.push_back(0.0f);

			vertices.push_back((j * tile_width) + tile_width);
			vertices.push_back((i * tile_height));
			vertices.push_back(0.0f);
		}
	}
}

void Tilemap::GenerateTilemapColorCoords(std::vector<float>& color_coords) {
	color_coords.clear();

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
}

void Tilemap::GenerateTilemapTextureCoords(std::vector<float>& tex_coords) {
	tex_coords.clear();

	for (unsigned int i = 0; i < grid_height; ++i) {
		for (unsigned int j = 0; j < grid_width; ++j) {
			//4 vertices per quad
			//Top left vertex
			tex_coords.push_back((tile_index_map[i][j][0] * tile_width));
			tex_coords.push_back((tile_index_map[i][j][1] * tile_height));

			//Bottom left vertex
			tex_coords.push_back((tile_index_map[i][j][0] * tile_width));
			tex_coords.push_back((tile_index_map[i][j][1] * tile_height) + tile_height);

			//Bottom right vertex
			tex_coords.push_back((tile_index_map[i][j][0] * tile_width) + tile_width);
			tex_coords.push_back((tile_index_map[i][j][1] * tile_height) + tile_height);

			//Top right vertex
			tex_coords.push_back((tile_index_map[i][j][0] * tile_width) + tile_width);
			tex_coords.push_back((tile_index_map[i][j][1] * tile_height));
		}
	}
}

//Sets the render mode to color (int=0) or texture (int=1)
void Tilemap::SetTextureMode(int _mode) {
	texture_mode = _mode;
}

glm::vec4 Tilemap::GetDimensions() {
	return dimensions;
}

int Tilemap::GetTileWidth() {
	return tile_width;
}
int Tilemap::GetTileHeight() {
	return tile_height;
}

int Tilemap::GetGridWidth() {
	return grid_width;
}

int Tilemap::GetGridHeight() {
	return grid_height;
}

void Tilemap::IncrementGridWidth() {
	std::vector<int> temp_tile = tile_index_map[0][grid_width-1];
	for (auto &row : tile_index_map) {
		row.push_back(temp_tile);
	}
	grid_width = tile_index_map[0].size(); //Number of columns in the tile_index_map
	dimensions.z = grid_width * tile_width;
}

void Tilemap::DecrementGridWidth() {
	for (auto& row : tile_index_map) {
		row.pop_back();
	}
	grid_width = tile_index_map[0].size(); //Number of columns in the tile_index_map
	dimensions.z = grid_width * tile_width;
}

void Tilemap::IncrementGridHeight() {
	std::vector<int> temp_tile = tile_index_map[grid_height-1][0];
	std::vector<std::vector<int>> temp_row;
	for (unsigned int i = 0; i < grid_width; ++i)
		temp_row.push_back(temp_tile);
	tile_index_map.push_back(temp_row);

	grid_height = tile_index_map.size(); //Number of rows in the tile_index_map
	dimensions.w = grid_height * tile_height;
}

void Tilemap::DecrementGridHeight() {
	tile_index_map.pop_back();
	grid_height = tile_index_map.size(); //Number of rows in the tile_index_map
	dimensions.w = grid_height * tile_height;
}

std::vector<std::vector<std::vector<int>>>& Tilemap::GetTileIndexMap() {
	return tile_index_map;
}

Texture* Tilemap::GetTexture()
{
	return p_texture;
}

std::vector<float> Tilemap::GetTexCoords() {
	return tex_coords;
}

void Tilemap::RegenTexCoords() {
	GenerateTilemapVertices(vertices);
	GenerateTilemapColorCoords(colors);
	GenerateTilemapTextureCoords(tex_coords);

	//Convert coords from image space to 0..1
	ConvertTextureCoords(tex_coords, p_texture->width, p_texture->height);
	
	//Delete and regen the vao
	p_graphics_manager->DeleteVAO(vao_id);
	vao_id = p_graphics_manager->GenerateQuadVAO(
		&vertices[0], &colors[0], &tex_coords[0], grid_width * grid_height);
}

json& Tilemap::GetObjMap()
{
	return obj_map;
}