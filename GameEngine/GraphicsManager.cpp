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
#include "FBO.h"
#include "InputManager.h"

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
									 window_height(WINDOW_HEIGHT), gamma(2.2f) {
	assert(GL_Initialize() == true);

	//ShaderProgram is a util class to encapsulate common methods and attributes for ShaderPrograms
	p_resource_manager->add_shader("final");
	CHECKERROR;
	p_resource_manager->add_shader("post");
	CHECKERROR;
	p_resource_manager->add_shader("ui");
	CHECKERROR;

	p_resource_manager->add_shader("fade");
	CHECKERROR;

	p_resource_manager->add_compute_shader("horizontal_blur");
	CHECKERROR;
	p_resource_manager->add_compute_shader("vertical_blur");
	CHECKERROR;

	SetActiveShader("post");
	BindAttrib(0, "in_vertices");
	
	SetActiveShader("final");
	BindDefaultAttribLocations();
	BindOutputAttrib(0, "out_Color");
	BindOutputAttrib(1, "post_Buffer");

	SetActiveShader("ui");
	BindDefaultAttribLocations();
	BindOutputAttrib(0, "out_Color");

	SetActiveShader("fade");
	BindDefaultAttribLocations();
	BindOutputAttrib(0, "out_Color");

	g_buffer = new FBO(window_width, window_height, 2);
	ping_pong_buffer = new FBO(window_width, window_height, 2);

	full_screen_quad_vao = GenerateFullScreenQuad();

	//float blur_weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
	std::vector<float> blur_weights = GenerateGaussianWeights(blur_kernel_width);
	GLuint bind_point = 0;
	GLuint h_block_id = GenerateUniformBlock(&blur_weights[0], sizeof(float) * blur_weights.size(), bind_point);
	SetActiveShader("horizontal_blur");
	BindBlockBinding(bind_point, "blurKernel");

	SetActiveShader("vertical_blur");
	BindBlockBinding(bind_point, "blurKernel");

	SetActiveShader("final");
	p_graphics_manager->SetAlphaBlendingOff();
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
	window_mode = 0;
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
	CHECKERROR;
	SetDepthTestOn();
	SetBlendingOn();
	CHECKERROR;
	
	return true;
}

/*Function to set the a shader program which
* will be used by the Graphics Manager for all subsequent operations
* Returns: void
*/
void GraphicsManager::SetActiveShader(const std::string& shader_name) {
	p_active_shader->Unuse();
	CHECKERROR;
	p_active_shader = p_resource_manager->get_shader(shader_name);
	CHECKERROR;
	p_active_shader->Use();
	CHECKERROR;
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
	glm::vec4 gamma_corrected(pow(clear_color.r, gamma), 
							  pow(clear_color.g, gamma),
							  pow(clear_color.b, gamma), 
							  pow(clear_color.a, gamma));
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_buffer->Bind();
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_buffer->Unbind();

	ping_pong_buffer->Bind();
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ping_pong_buffer->Unbind();
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
	std::vector<GLuint> temp_id_list;

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
	temp_id_list.push_back(point_buffer);

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
	temp_id_list.push_back(color_buffer);

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
	temp_id_list.push_back(tex_coord_buffer);

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
	temp_id_list.push_back(indeces_buffer);

	//Store the vao_id with the buffer ids for deletion later.
	vao_map[vao_id] = temp_id_list;

	return vao_id;
}

/*Creates a vertex array object with a dynamic array for the vertices
* and sends it to the GPU
* Returns: GLuint - The vao_id
*/
GLuint GraphicsManager::GenerateDynamicQuadVAO(GLuint& vertex_buffer_id, 
	GLuint& color_buffer_id, GLuint& texture_buffer_id, 
	unsigned int batch_size) {
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
	glGenBuffers(1, &color_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * vertices_count * colors_count * batch_size,
		NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Create another continguous buffer for all the textures for each vertex
	glGenBuffers(1, &texture_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * vertices_count * tex_coords_count * batch_size,
		NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//IBO data
	std::vector<GLuint> indexData;
	for (unsigned int i = batch_size-1; i > 0; --i) {
		indexData.push_back(0 + (i * 4));
		indexData.push_back(1 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(0 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(3 + (i * 4));
	}

	indexData.push_back(0);
	indexData.push_back(1);
	indexData.push_back(2);
	indexData.push_back(0);
	indexData.push_back(2);
	indexData.push_back(3);
	
	/*
	std::vector<GLuint> indexData = { 0, 1, 2, 0, 2, 3 };
	for (unsigned int i = 1; i < batch_size; ++i) {
		indexData.push_back(0 + (i * 4));
		indexData.push_back(1 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(0 + (i * 4));
		indexData.push_back(2 + (i * 4));
		indexData.push_back(3 + (i * 4));
	}*/

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
void GraphicsManager::BindAttrib(GLuint attrib, const std::string& var_name) {
	//Attrib location 0 will always be used for position coordinates
	glBindAttribLocation(p_active_shader->program_id, attrib, var_name.c_str());
	CHECKERROR;
}

void GraphicsManager::BindOutputAttrib(GLuint attrib, const std::string& var_name) {
	glBindFragDataLocation(p_active_shader->program_id, attrib, var_name.c_str());
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
void GraphicsManager::DrawQuad(GLuint vao_id, unsigned int batch_size, bool gbuffer_draw) {
	if (gbuffer_draw)
		g_buffer->Bind();

	glBindVertexArray(vao_id);
	CHECKERROR;
	
	SetUniformFloat(gamma, "gamma");

	if (gbuffer_draw) {
		GLenum bufs[2] = { GL_COLOR_ATTACHMENT0_EXT , GL_COLOR_ATTACHMENT1_EXT };
		glDrawBuffers(2, bufs);
		CHECKERROR;
	}

	glDrawElements(GL_TRIANGLES, 6 * batch_size, GL_UNSIGNED_INT, NULL);
	CHECKERROR;
	glBindVertexArray(0);

	if (gbuffer_draw)
		g_buffer->Unbind();
}

//Sets a uniform int
void GraphicsManager::SetUniformInt(int var, const std::string& var_name) {
	CHECKERROR;
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform1i(loc, var);
	CHECKERROR;
}

//Sets a uniform float
void GraphicsManager::SetUniformFloat(float var, const std::string& var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform1f(loc, var);
	CHECKERROR;
}

//Sets a uniform matrix4
void GraphicsManager::SetUniformMatrix4(glm::mat4 const &var, const std::string& var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(var));
	CHECKERROR;
}

//Sets a uniform vec2
void GraphicsManager::SetUniformVec2(glm::vec2 const& var, const std::string& var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform2fv(loc, 1, &(var[0]));
	CHECKERROR;
}

//Sets a uniform vec3
void GraphicsManager::SetUniformVec3(glm::vec3 const& var, const std::string& var_name) {
	GLuint loc = glGetUniformLocation(p_active_shader->program_id, var_name.c_str());
	glUniform3fv(loc, 1, &(var[0]));
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
	glDepthMask(GL_TRUE);
}

//Set Depth test off
void GraphicsManager::SetDepthTestOff() {
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void GraphicsManager::SetAlphaBlendingOn() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void GraphicsManager::SetAlphaBlendingOff() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GraphicsManager::SetColorBlendingOn() {
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
}

void GraphicsManager::SetColorBlendingOff() {
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
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

void GraphicsManager::SetOrthographicMatrix() {
	float right = window_width;
	float bottom = window_height;
	glm::mat4 ortho_proj = glm::ortho(0.0f, right, bottom, 0.0f, 0.0f, 1.0f);
	SetUniformMatrix4(ortho_proj, "ortho_projection");
}

//Performs all the post processing tasks required. 
void GraphicsManager::PostProcess() {
	BlurBuffer(g_buffer, 1, 10);
}

/*Draw a full screen quad and render the GBuffer
* Returns: void
*/
void GraphicsManager::DrawGBuffer() {
	SetActiveShader("post");

	glBindVertexArray(full_screen_quad_vao);
	CHECKERROR;

	if (p_input_manager->isKeyPressed(SDL_SCANCODE_R))
		SetUniformInt(1, "debug");
	else if (p_input_manager->isKeyPressed(SDL_SCANCODE_T))
		SetUniformInt(2, "debug");
	else
		SetUniformInt(0, "debug");

	g_buffer->BindTexture(GetActiveShader()->program_id, 2, "gBuffer", 0);
	g_buffer->BindTexture(GetActiveShader()->program_id, 4, "gBuffer_d", 1);
	ping_pong_buffer->BindTexture(GetActiveShader()->program_id, 3, "bloomBuffer", 1);
	CHECKERROR;

	SetUniformInt(window_width, "width");
	SetUniformInt(window_height, "height");

	SetUniformFloat(gamma, "gamma");

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	CHECKERROR;
	glBindVertexArray(0);

	SetActiveShader("final");
}

/*Create a quad that fills the entire screen.
* Used to render the gbuffer
* Returns: GLuint - vao_id of the quad
*/
GLuint GraphicsManager::GenerateFullScreenQuad() {
	std::vector<GLuint> temp_id_list;

	float vertices[12] = {
		-1,  1, 0,
		-1, -1, 0,
		 1, -1, 0,
		 1,  1, 0
	};

	//Create a VAO and put the ID in vao_id
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Create a continguous buffer for all the vertices/points
	GLuint point_buffer;
	glGenBuffers(1, &point_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	temp_id_list.push_back(point_buffer);
	//IBO data
	GLuint indeces[6] = {0, 1, 2, 0, 2, 3};
	//Create IBO
	GLuint indeces_buffer;
	glGenBuffers(1, &indeces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indeces, 
				 GL_STATIC_DRAW);
	CHECKERROR;
	temp_id_list.push_back(indeces_buffer);

	glBindVertexArray(0);

	vao_map[vao_id] = temp_id_list;
	return vao_id;
}

/* Perform a blur on a given frame buffer
* Stores the blurred image in the ping_pong_buffer
* Returns: void
*/
void GraphicsManager::BlurBuffer(FBO* fbo, GLuint color_attachment, int iterations) {
	GLuint input_imageUnit = 0;
	GLuint output_imageUnit = 1;
	GLuint fbo_tex_id;
	GLuint loc;
	for (unsigned int i = 0; i < iterations; ++i) {
		SetActiveShader("horizontal_blur");
		SetUniformInt(blur_kernel_width, "width");

		if (i == 0) {
			//Use the input fbo as the input for the first iteration
			fbo_tex_id = fbo->textureID[color_attachment];
			glBindImageTexture(input_imageUnit, fbo_tex_id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
			loc = glGetUniformLocation(GetActiveShader()->program_id, "src");
			glUniform1i(loc, input_imageUnit);
			CHECKERROR;
		}
		else {
			//Use the ping pong buffer as the input for the other iterations
			fbo_tex_id = ping_pong_buffer->textureID[1];
			glBindImageTexture(input_imageUnit, fbo_tex_id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
			loc = glGetUniformLocation(GetActiveShader()->program_id, "src");
			glUniform1i(loc, input_imageUnit);
			CHECKERROR;
		}
		

		fbo_tex_id = ping_pong_buffer->textureID[0];
		glBindImageTexture(output_imageUnit, fbo_tex_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		loc = glGetUniformLocation(GetActiveShader()->program_id, "dst");
		glUniform1i(loc, output_imageUnit);
		CHECKERROR;

		glDispatchCompute(glm::ceil(window_width / 128.0f), window_height, 1);
		CHECKERROR;

		SetActiveShader("vertical_blur");
		SetUniformInt(blur_kernel_width, "width");

		fbo_tex_id = ping_pong_buffer->textureID[0];
		glBindImageTexture(input_imageUnit, fbo_tex_id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		loc = glGetUniformLocation(GetActiveShader()->program_id, "src");
		glUniform1i(loc, input_imageUnit);
		CHECKERROR;

		fbo_tex_id = ping_pong_buffer->textureID[1];
		glBindImageTexture(output_imageUnit, fbo_tex_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		loc = glGetUniformLocation(GetActiveShader()->program_id, "dst");
		glUniform1i(loc, output_imageUnit);
		CHECKERROR;

		glDispatchCompute(window_width, glm::ceil(window_height / 128.0f), 1);
		CHECKERROR;
	}

	SetActiveShader("final");
}

/*Create a uniform block and send data to it 
* Returns: GLuint - block_id for the created block
*/
GLuint GraphicsManager::GenerateUniformBlock(float const* block_data, size_t block_size, GLuint bind_point) {
	GLuint blockID;
	glGenBuffers(1, &blockID);

	glBindBuffer(GL_UNIFORM_BUFFER, blockID);
	glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, blockID);
	glBufferData(GL_UNIFORM_BUFFER, block_size, block_data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CHECKERROR;

	return blockID;
}

/* Bind a block to a bindpoint for the active shader
* Returns: void
*/
void GraphicsManager::BindBlockBinding(GLuint bind_point, const std::string& block_name) {
	GLuint loc = glGetUniformBlockIndex(GetActiveShader()->program_id, block_name.c_str());
	glUniformBlockBinding(GetActiveShader()->program_id, loc, bind_point);
}

/*Function to delete a VAO
* Returns: void
*/
void GraphicsManager::DeleteVAO(GLuint vao_id) {
	//Bind the vao before deleting the buffers that are part of the vao
	glBindVertexArray(vao_id);
	for (auto buffer_id : vao_map[vao_id])
		DeleteBufferObject(buffer_id);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao_id);
	CHECKERROR;
}

void GraphicsManager::DeleteBufferObject(GLuint buffer_id) {
	glDeleteBuffers(1, &buffer_id);
}

/*Makes a texture accessible to a active shader program
* Returns: void
*/
void GraphicsManager::BindTexture(const int unit, const GLuint tex_id, const std::string& name)
{
	glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
	CHECKERROR;
	glBindTexture(GL_TEXTURE_2D, tex_id);
	CHECKERROR;
	int loc = glGetUniformLocation(GetActiveShader()->program_id, name.c_str());
	CHECKERROR;
	glUniform1i(loc, unit);
	CHECKERROR;
}

/*Change from windowed mode with fixed resolution
* to fullscreen mode
* Returns: void
*/
void GraphicsManager::EnterFullScreenMode() {
	SDL_SetWindowFullscreen(p_sdl_window, SDL_WINDOW_FULLSCREEN);
	SDL_DisplayMode mode;
	
	SDL_assert(SDL_GetWindowDisplayMode(p_sdl_window, &mode) == 0);

	window_width = mode.w;
	window_height = mode.h;
	window_mode = SDL_WINDOW_FULLSCREEN;

	glViewport(0, 0, window_width, window_height);
	RegenFBOs();
}

/*Change from full screen mode to windowed mode
* Returns: void
*/
void GraphicsManager::EnterWindowedMode() {
	SDL_SetWindowFullscreen(p_sdl_window, 0);
	SDL_DisplayMode mode;

	SDL_assert(SDL_GetWindowDisplayMode(p_sdl_window, &mode) == 0);

	window_width = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;

	window_mode = 0;

	glViewport(0, 0, window_width, window_height);
	RegenFBOs();
}

/*Deletes and recreates FBOs with the new
* window width and height
* Also re-creates the fullscreen quad
*/
void GraphicsManager::RegenFBOs() {
	delete g_buffer;
	delete ping_pong_buffer;
	g_buffer = new FBO(window_width, window_height, 2);
	ping_pong_buffer = new FBO(window_width, window_height, 2);

	DeleteVAO(full_screen_quad_vao);
	full_screen_quad_vao = GenerateFullScreenQuad();
}
