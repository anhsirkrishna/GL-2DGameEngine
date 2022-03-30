/******************************************************************************/
/*!
/*File   Text.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  Text header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <glm.hpp>

typedef float GLfloat;
typedef unsigned int GLuint;

class Transform;
class CharacterTextures;

class UIText : public Component {
private:
	std::string text;
	glm::vec2 pixel_position;
	glm::vec2 scale;
	std::vector<std::vector<GLfloat>> vertex_coords;
	std::vector<GLfloat> color_coords;
	std::vector<GLfloat> tex_coords;
	GLuint vertex_buffer_id, color_buffer_id, texture_buffer_id;
	GLuint vao_id;
	bool visible;
	int texture_mode;
	Transform* p_owner_transform;
	CharacterTextures* p_font_textures;
public:
	UIText();

	/*Draw the UIText on the screen with an
	* orthographic projection
	* Returns: void
	*/
	virtual void Draw(ShaderProgram* p_program);

	/*Update function called once per loop
	* Returns: void
	*/
	virtual void Update();

	/*Serialize the UIText from a json object
	* Expects a dict with the following key:values
	* pixel_position : [float, float]
	* font_name : string
	* pixel_scale : [float, float]
	* text : string
	* Returns: void
	*/
	virtual void Serialize(json json_object);

	virtual void Link();

	/*Populates the vertex array object for
	* vertex coords
	* Returns: void
	*/
	void GenerateTilemapVertices();
};