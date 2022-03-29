/******************************************************************************/
/*!
/*File   Button.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  Button header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <glm.hpp>

class Texture;
class Behavior;
class Transform;

typedef float GLfloat;
typedef unsigned int GLuint;

class Button : public Component {
private:
	glm::vec2 pixel_position;
	glm::vec2 scale;
	glm::vec2 pixel_dimensions;
	glm::vec2 quad_pixel_dimensions;
	Texture* p_texture;
	std::vector<GLfloat> vertex_coords;
	std::vector<GLfloat> color_coords;
	std::vector<GLfloat> tex_coords;
	std::vector<GLfloat> clicked_tex_coords;
	GLuint vertex_buffer_id, color_buffer_id, texture_buffer_id;
	std::vector<std::vector<std::vector<GLuint>>> tile_map_index;
	std::vector<std::vector<std::vector<GLuint>>> clicked_tile_map_index;
	GLuint vao_id;
	void (*button_action)();
	bool visible;
	bool button_down;
	int grid_width;
	int grid_height;
	int texture_mode;
	Behavior* p_behavior;
	Transform* p_owner_transform;
public:
	Button();
	~Button();
	/*Draw the button on the screen with an 
	* orthographic projection
	* Returns: void
	*/
	virtual void Draw(ShaderProgram* p_program);
	/*Update function called once per loop
	* Returns: void
	*/
	virtual void Update();
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
	virtual void Serialize(json json_object);

	virtual void Link();
	/*Sets the action to perform when the button is clicked
	* Returns: void
	*/
	void SetButtonAction(void (*_button_action)());
	/*Call the button action function
	* Returns: void
	*/
	void Click();

	/*Populates the vertex array object for 
	* vertex coords
	* Returns: void
	*/
	void GenerateTilemapVertices();

	/*Populates the vertex array object for
	* texture coords
	* Returns: void
	*/
	void GenerateTilemapTextureCoords();
};