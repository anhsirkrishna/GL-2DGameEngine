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

class GLQuad;
class Transform;

class Tilemap : public Component {
private:
	GLQuad* p_owner_sprite;
	Transform* p_owner_transform;
	int grid_width;
	int grid_height;
	int tile_width;
	int tile_height;
	glm::vec4 dimensions;

	std::vector<std::vector<std::vector<int>>> tile_index_map;
public:
	Tilemap();
	void Serialize(json json_object);
	virtual void Link();
	virtual void Draw(ShaderProgram* p_program);
};