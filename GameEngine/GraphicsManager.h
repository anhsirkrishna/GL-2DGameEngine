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
class FBO;

class GraphicsManager {
private:
	ShaderProgram* p_active_shader;
	FBO* g_buffer;
	GLuint full_screen_quad_vao;
	FBO* ping_pong_buffer;

	//variable for gamma correction
	float gamma;

	int blur_kernel_width=10;

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
	GLuint window_width;
	GLuint window_height;

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

	/*Creates a vertex array object with a dynamic array for the vertices 
	* and sends it to the GPU
	* Returns: GLuint - The vao_id
	*/
	GLuint GenerateDynamicQuadVAO(GLuint &vertex_buffer_id, GLuint& color_buffer_id,
		float const* texture_coords, unsigned int batch_size = 1);

	/*Dynamically set the data for a GL vertex buffer and send to the GPU
	* Returns: void
	*/
	void SetDynamicBufferData(GLuint vao_id, GLuint vertex_buffer_id, 
		float const* data, size_t data_size);

	/*Creates a dynamic array buffer associated with a vao_id
	* and sends it to the GPU
	* Returns: GLuint the id of the dynamic array buffer
	*/
	GLuint GenerateDynamicArrayBuffer(GLuint vao_id, GLuint array_count, 
		size_t array_size, GLuint attrib);

	/*Function to bind an attribute locations
	* for the given variable
	* Returns: void
	*/
	void BindAttrib(GLuint attrib, std::string var_name);

	/*Function to bind an attribute location for output
	* for a given variable
	* Returns: void
	*/
	void BindOutputAttrib(GLuint attrib, std::string var_name);

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

	//Sets a uniform vec3
	void SetUniformVec3(glm::vec3 const& var, std::string var_name);

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

	//Set Projection Matrix
	void SetProjectionMatrix();

	//Set view Matrix
	void SetViewMatrix();

	//Performs all the post processing tasks required. 
	void PostProcess();

	/*Draw a full screen quad and render the GBuffer
	* Returns: void
	*/
	void DrawGBuffer();

	/*Create a quad that fills the entire screen.
	* Used to render the gbuffer
	* Returns: GLuint - vao_id of the quad
	*/
	GLuint GenerateFullScreenQuad();

	/* Perform a blur on a given frame buffer
	* Stores the blurred image in the ping_pong_buffer
	* Returns: void
	*/
	void BlurBuffer(FBO* fbo, GLuint color_attachment, int iterations);

	/* Bind a block to a bindpoint for the active shader
	* Returns: void
	*/
	void BindBlockBinding(GLuint bind_point, std::string block_name);

	/*Create a uniform block and send data to it
	* Returns: GLuint - block_id for the created block
	*/
	GLuint GenerateUniformBlock(float const* block_data, size_t block_size, GLuint bind_point);

	//Set color blending as additive
	void SetColorBlendingOn();

	//Set color blending off
	void SetColorBlendingOff();

	/*Function to delete a vao
	* Returns: void
	*/
	void DeleteVAO(GLuint vao_id);
};

extern GraphicsManager* p_graphics_manager;