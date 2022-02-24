/******************************************************************************/
/*!
/*File   GraphicsManager.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Graphics Manager implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include <assert.h>

#include "GraphicsManager.h"
#include "GameDefs.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Util.h"
#include "Camera.h"

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <SDL_image.h>
#include <glm.hpp>

/*
* Macro used to check for OpenGL errors.
* Use this often to ensure all the OpenGL calls go through cleanly
* without any errors
*/
#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line GraphicsManager.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }

GLuint ATTRIB_POSITION = 0;
GLuint ATTRIB_COLOR = 1;
GLuint ATTRIB_TEX_COORD = 2;

/*Initializes the graphics manager
* Creates all the shader programs necessary
*/
GraphicsManager::GraphicsManager() : p_active_shader(nullptr), p_sdl_window(nullptr), 
									 p_gl_context(nullptr), window_width(WINDOW_WIDTH), 
									 window_height(WINDOW_HEIGHT) {
	assert(GL_Initialize() == true);

	//ShaderProgram is a util class to encapsulate common methods and attributes for ShaderPrograms
	p_resource_manager->add_shader("final");
	SetActiveShader("final");
	BindDefaultAttribLocations();
}

GraphicsManager::~GraphicsManager() {
	// Close and destroy the window
	SDL_DestroyWindow(p_sdl_window);
}

/*
* Function to initalize SDL and Open GL and SDL_Image
* Sets the OpenGL Version
* Creates an SDL Window
* Associates an OpenGL Context
* Init GLEW framework
* Init PNG loading using SDL_image
* Returns: bool True if Init succeeded
*/
bool GraphicsManager::GL_Initialize() {
	bool success = true;
	int error = 0;
	// Initialize SDL
	if ((error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)) < 0)
	{
		SDL_Log("Couldn't initialize SDL, error %i\n", error);
		return false;
	}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0)
	{
		// In case the required Depth buffer couldn't be acquired
		SDL_Log("Could not allocated required depth buffer: %s\n",
			SDL_GetError());
		return false;
	}

	p_sdl_window = SDL_CreateWindow("GameEngine",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		window_width,								// width, in pixels
		window_height,								// height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Check that the window was successfully made
	if (NULL == p_sdl_window)
	{
		// In the event that the window could not be made...
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	//Create context
	p_gl_context = SDL_GL_CreateContext(p_sdl_window);
	if (p_gl_context == NULL)
	{
		SDL_Log("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Initialize GLEW
		GLenum glew_error = glewInit();
		if (glew_error != GLEW_OK)
		{
			SDL_Log("Error initializing GLEW! %s\n", glewGetErrorString(glew_error));
			return false;
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			SDL_Log("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Initialize PNG loading
		int img_flags = IMG_INIT_PNG;
		if (!(IMG_Init(img_flags) & img_flags))
		{
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}
	}

	int depth_size;
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depth_size);
	
	SetDepthTestOn();
	SetBlendingOn();

	return true;
}

/*Function to set the a shader program which
* will be used by the Graphics Manager for all subsequent operations
* Returns: void
*/
void GraphicsManager::SetActiveShader(std::string shader_name) {
	p_active_shader->Unuse();
	p_active_shader = p_resource_manager->get_shader(shader_name);
	p_active_shader->Use();
}

/*Function to get the a shader program which
* is used by the Graphics Manager
* Returns: ShaderProgram*
*/
ShaderProgram* GraphicsManager::GetActiveShader() {
	return p_active_shader;
}

/*Function to bind the default attribute locations
* for position, color and tex_coords.
* Returns: void
*/
void GraphicsManager::BindDefaultAttribLocations() {
	//Attrib location 0 will always be used for position coordinates
	glBindAttribLocation(p_active_shader->program_id, ATTRIB_POSITION, "in_position");
	//Attrib location 1 will always be used for colors
	glBindAttribLocation(p_active_shader->program_id, ATTRIB_COLOR, "in_color");
	//Attric location 2 will always be used for texture coordinates
	glBindAttribLocation(p_active_shader->program_id, ATTRIB_TEX_COORD, "in_texcoords");
	CHECKERROR;
}

/*Clear the currently active buffer
* Returns: void
*/
void GraphicsManager::ClearBuffer(glm::vec4 clear_color) {
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*Swap the currently active buffer
* with the inactive buffer
* Returns: void
*/
void GraphicsManager::SwapBuffers() {
	SDL_GL_SwapWindow(p_sdl_window);
}

/*Creates a vertex array object and sends it to the GPU
* Returns: GLuint - The vao_id
*/
GLuint GraphicsManager::GenerateQuadVAO(float const* positions, 
	float const* colors, float const* texture_coords, unsigned int batch_size) {
	int vertices_count = 4;
	int positions_count = 3;
	int colors_count = 4;
	int tex_coords_count = 2;
	//Create a VAO and put the ID in vao_id
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Create a continguous buffer for all the vertices/points
	GLuint point_buffer;
	glGenBuffers(1, &point_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		         sizeof(float) * vertices_count * positions_count * batch_size,
				 positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Create another continuguous buffer for all the colors for each vertex
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		         sizeof(float) * vertices_count * colors_count * batch_size,
				 colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Create another continguous buffer for all the textures for each vertex
	GLuint tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(float) * vertices_count * tex_coords_count * batch_size,
				 texture_coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//IBO data
	std::vector<GLuint> indexData = { 0, 1, 2, 0, 2, 3 };
	for (unsigned int i = 1; i < batch_size; ++i) {
		indexData.push_back(0 + (i * 4));
		indexData.push_back(1 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(0 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(3 + (i * 4));
	}
	//Create IBO
	GLuint indeces_buffer;
	glGenBuffers(1, &indeces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint) * batch_size,
				 &indexData[0], GL_STATIC_DRAW);
	CHECKERROR;
	glBindVertexArray(0);

	return vao_id;
}

/*Creates a vertex array object with a dynamic array for the vertices
* and sends it to the GPU
* Returns: GLuint - The vao_id
*/
GLuint GraphicsManager::GenerateDynamicQuadVAO(GLuint& vertex_buffer_id,
	float const* colors, float const* texture_coords, unsigned int batch_size) {
	int vertices_count = 4;
	int positions_count = 3;
	int colors_count = 4;
	int tex_coords_count = 2;
	//Create a VAO and put the ID in vao_id
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Create a continguous buffer for all the vertices/points
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * vertices_count * positions_count * batch_size,
		NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Create another continuguous buffer for all the colors for each vertex
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * vertices_count * colors_count * batch_size,
		colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Create another continguous buffer for all the textures for each vertex
	GLuint tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * vertices_count * tex_coords_count * batch_size,
		texture_coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//IBO data
	std::vector<GLuint> indexData = { 0, 1, 2, 0, 2, 3 };
	for (unsigned int i = 1; i < batch_size; ++i) {
		indexData.push_back(0 + (i * 4));
		indexData.push_back(1 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(0 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(3 + (i * 4));
	}
	//Create IBO
	GLuint indeces_buffer;
	glGenBuffers(1, &indeces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint) * batch_size,
		&indexData[0], GL_STATIC_DRAW);
	CHECKERROR;
	glBindVertexArray(0);

	return vao_id;
}

/*Creates a dynamic array buffer associated with a vao_id
* and sends it to the GPU
* Returns: GLUint the array buffer id
*/
GLuint GraphicsManager::GenerateDynamicArrayBuffer(GLuint vao_id, GLuint array_count, size_t array_size, GLuint attrib) {
	glBindVertexArray(vao_id);
	CHECKERROR;

	//Create a continguous buffer for the array
	GLuint new_array_buffer;
	glGenBuffers(1, &new_array_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, new_array_buffer);
	glBufferData(GL_ARRAY_BUFFER, array_size, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, array_count, GL_FLOAT, GL_FALSE, 0, 0);
	CHECKERROR;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	glBindVertexArray(0);

	return new_array_buffer;
}

/*Function to bind an attribute locations
* for the given variable
* Returns: void
*/
void GraphicsManager::BindAttrib(GLuint attrib, std::string var_name) {
	//Attrib location 0 will always be used for position coordinates
	glBindAttribLocation(p_active_shader->program_id, attrib, var_name.c_str());
	CHECKERROR;
}

/*Dynamically set the data for a GL vertex buffer and send to the GPU
* Returns: void
*/
void GraphicsManager::SetDynamicBufferData(GLuint vao_id, GLuint vertex_buffer_id, 
	float const* data, size_t data_size) {
	glBindVertexArray(vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	CHECKERROR;
}

/*Sends the GL_Draw call after binding the specified vao
* Returns: void
*/
void GraphicsManager::DrawQuad(GLuint vao_id, unsigned int batch_size) {
	glBindVertexArray(vao_id);
	CHECKERROR;
	glDrawElements(GL_TRIANGLES, 6 * batch_size, GL_UNSIGNED_INT, NULL);
	CHECKERROR;
	glBindVertexArray(0);
}

//Sets a uniform int
void GraphicsManager::SetUniformInt(int var, std::string var_name) {
	CHECKERROR;
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform1i(loc, var);
	CHECKERROR;
}

//Sets a uniform float
void GraphicsManager::SetUniformFloat(float var, std::string var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform1f(loc, var);
	CHECKERROR;
}

//Sets a uniform matrix4
void GraphicsManager::SetUniformMatrix4(glm::mat4 const &var, std::string var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(var));
	CHECKERROR;
}

//Sets a uniform vec2
void GraphicsManager::SetUniformVec2(glm::vec2 const& var, std::string var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform2fv(loc, 1, &(var[0]));
	CHECKERROR;
}


//Set blending on
void GraphicsManager::SetBlendingOn() {
	glEnable(GL_BLEND);
}

//Set blending off
void GraphicsManager::SetBlendingOff() {
	glDisable(GL_BLEND);
}

//Set Depth test on
void GraphicsManager::SetDepthTestOn() {
	glEnable(GL_DEPTH_TEST);
}

//Set Depth test off
void GraphicsManager::SetDepthTestOff() {
	glDisable(GL_DEPTH_TEST);
}

void GraphicsManager::SetAlphaBlendingOn() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void GraphicsManager::SetAlphaBlendingOff() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GraphicsManager::SetProjectionMatrix() {
	// set up projection matrix for the camera
	glm::mat4 projection = glm::perspective(glm::radians(p_camera->zoom), (float)window_width / (float)window_height, 0.1f, 10000.0f);
	SetUniformMatrix4(projection, "projection");
}

void GraphicsManager::SetViewMatrix() {
	// set up view matrix for the camera
	glm::mat4 view = p_camera->GetViewMatrix();
	SetUniformMatrix4(view, "view");
}