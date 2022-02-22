/******************************************************************************/
/*!
/*File   GraphicsManager.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Graphics Manager header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once
#include <string>
#include <glm.hpp>

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef void* SDL_GLContext;

class ShaderProgram;
struct SDL_Window;

class GraphicsManager {
private:
	ShaderProgram* p_active_shader;
	GLuint window_width;
	GLuint window_height;
	/*
	* Function to initalize SDL and Open GL and SDL_Image
	* Sets the OpenGL Version
	* Creates an SDL Window
	* Associates an OpenGL Context
	* Init GLEW framework
	* Init PNG loading using SDL_image
	* Returns: bool True if Init succeeded
	*/
	bool GL_Initialize();
public:
	SDL_Window* p_sdl_window;
	SDL_GLContext p_gl_context;

	/*Initializes the graphics manager
	* Creates all the shader programs necessary
	*/
	GraphicsManager();

	~GraphicsManager();

	/*Function to set the a shader program which
	* will be used by the Graphics Manager for all subsequent operations
	* Returns: void
	*/
	void SetActiveShader(std::string shader_name);

	/*Function to get the a shader program which
	* is used by the Graphics Manager
	* Returns: ShaderProgram*
	*/
	ShaderProgram* GetActiveShader();

	/*Function to bind the default attribute locations
	* for position, color and tex_coords.
	* Returns: void
	*/
	void BindDefaultAttribLocations();

	/*Creates a vertex array object and sends it to the GPU
	* Returns: GLuint - The vao_id
	*/
	GLuint GenerateQuadVAO(float const *positions, float const *colors,
						   float const *texture_coords, 
						   unsigned int batch_size=1);


	/*Sends the GL_Draw call after binding the specified vao
	* Returns: void
	*/
	void DrawQuad(GLuint vao_id, unsigned int batch_size=1);

	/*Clear the currently active buffer
	* Returns: void
	*/
	void ClearBuffer(glm::vec4 clear_color);

	//Swap the buffer in the double buffers
	void SwapBuffers();

	//Sets a uniform int
	void SetUniformInt(int var, std::string var_name);

	//Sets a uniform float
	void SetUniformFloat(float var, std::string var_name);

	//Sets a uniform matrix4
	void SetUniformMatrix4(glm::mat4 const &var, std::string var_name);

	//Sets a uniform vec2
	void SetUniformVec2(glm::vec2 const &var, std::string var_name);

	//Set blending on
	void SetBlendingOn();

	//Set blending off
	void SetBlendingOff();

	//Set Depth test on
	void SetDepthTestOn();

	//Set Depth test off
	void SetDepthTestOff();

	//Set alpha blending as additive
	void SetAlphaBlendingOn();

	//Set alpha blending to default 
	void SetAlphaBlendingOff();
};

extern GraphicsManager* p_graphics_manager;