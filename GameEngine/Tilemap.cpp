
#include "Tilemap.h"
#include "GLQuad.h"
#include "Transform.h"
#include "GameObject.h"

Tilemap::Tilemap() : Component("TILEMAP"), p_owner_sprite(NULL), p_owner_transform(NULL), grid_width(0),
grid_height(0), tile_width(0), tile_height(0), dimensions() {}

void Tilemap::Serialize(json json_object) {
	//The tile index map is a 2D array with each element containing [tex_offset_x, tex_offset_y, tex_index]
	tile_index_map = json_object["tile_index_map"].get<std::vector<std::vector<std::vector<int>>>>();
	tile_width = json_object["tile_width"].get<int>();
	tile_height = json_object["tile_height"].get<int>();
	grid_width = tile_index_map[0].size(); //Number of columns in the tile_index_map
	grid_height = tile_index_map.size(); //Number of rows in the tile_index_map
	auto position = json_object["position"].get<std::vector<int>>();

	dimensions.x = position[0];
	dimensions.y = position[1];
	dimensions.z = grid_width * tile_width;
	dimensions.w = grid_width * tile_height;
}

void Tilemap::Link() {
	p_owner_sprite = static_cast<GLQuad*>(GetOwner()->HasComponent("GLQUAD"));
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

void Tilemap::Draw(ShaderProgram* p_program) {
	int tex_offset_x, tex_offset_y;
	glm::ivec4 grid(0, 0, p_owner_transform->GetPosition().z, 1);
	for (auto row : tile_index_map) {
		for (auto tile : row) {
			p_owner_transform->SetPosition(grid);
			p_owner_transform->Update();
			tex_offset_x = tile[0] * tile_width;
			tex_offset_y = tile[1] * tile_height;
			p_owner_sprite->SetTexOffset(tex_offset_x, tex_offset_y);
			p_owner_sprite->Draw(p_program);
			grid.x += tile_width;
		}
		grid.x = 0;
		grid.y += tile_height;
	}
}