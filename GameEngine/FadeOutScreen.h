/******************************************************************************/
/*!
/*File   FadeOutScreen.h
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

class FadeScreen : public Component {
private:
	float fade_time;
	float timer;
	float transparency_step;
	float transparency;
	bool completed;
	bool fade_out;
	glm::vec4 fade_color;
	GLuint quad_vao_id;
	std::vector<GLfloat> vertex_coords;
	std::vector<GLfloat> color_coords;
	std::vector<GLfloat> tex_coords;
	GLuint vertex_buffer_id, color_buffer_id, texture_buffer_id;
public:
	FadeScreen(unsigned int _fade_time, glm::vec4 _color, bool _fade_out=true);

	virtual void Draw(ShaderProgram* p_program);
	/*Update function called once per loop
	* Returns: void
	*/
	virtual void Update();

	/*Check if the fade has completed*/
	bool isComplete();
};