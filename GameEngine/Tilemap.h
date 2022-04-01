/******************************************************************************/
/*!
/*File   Tilemap.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Tilemap component header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include <glm.hpp>

#include "Component.h"

typedef unsigned int GLuint;

class GLQuad;
class Texture;
class Transform;

class Tilemap : public Component {
private:
	GLuint vao_id;
	Texture* p_texture;
	Transform* p_owner_transform;
	int grid_width;
	int grid_height;
	int tile_width;
	int tile_height;
	glm::vec4 dimensions;
	glm::vec3 brightness;
	int texture_mode;
	std::vector<std::vector<std::vector<int>>> tile_index_map;

	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<float> tex_coords;

	json obj_map;

	void GenerateTilemapVertices(std::vector<float> &vertices);
	void GenerateTilemapTextureCoords(std::vector<float>& tex_coords);
public:
	Tilemap();
	void Serialize(json json_object);
	virtual void Link();
	virtual void Draw(ShaderProgram* p_program);
	void SetTextureMode(int _mode);
	glm::vec4 GetDimensions();
	int GetTileWidth();
	int GetTileHeight();
	int GetGridWidth();
	int GetGridHeight();
	std::vector<std::vector<std::vector<int>>>& GetTileIndexMap();
	Texture* GetTexture();
	std::vector<float> GetTexCoords();
	void RegenTexCoords();
	json& GetObjMap();
};