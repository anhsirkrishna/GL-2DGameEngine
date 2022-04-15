/******************************************************************************/
/*!
/*File   LogoScreen.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  Button header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "Component.h"

#include <glm.hpp>

typedef unsigned int GLuint;
typedef float GLfloat;

class Texture;

class LogoScreen : public Component {
private:
	float fade_time;
	float screen_time;
	float timer;
	float transparency_step;
	float transparency;
	bool completed;
	bool fade_out;
	glm::vec4 back_color;
	//Background quad
	GLuint background_vao_id;
	std::vector<GLfloat> back_vertex_coords;
	std::vector<GLfloat> back_color_coords;
	std::vector<GLfloat> back_tex_coords;
	GLuint back_vertex_buffer_id, back_color_buffer_id, back_texture_buffer_id;

	//Logo Image
	GLuint image_vao_id;
	std::vector<GLfloat> vertex_coords;
	std::vector<GLfloat> color_coords;
	std::vector<GLfloat> tex_coords;
	GLuint vertex_buffer_id, color_buffer_id, texture_buffer_id;
	Texture* p_logo_tex;
public:
	LogoScreen(unsigned int _fade_time, unsigned int _screen_time, 
				glm::vec4 _color, std::string tex_name, 
				bool _fade_out = true);

	virtual void Draw(ShaderProgram* p_program);
	/*Update function called once per loop
	* Returns: void
	*/
	virtual void Update();

	/*Check if the fade has completed*/
	bool isComplete();
};