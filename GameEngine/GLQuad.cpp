#include "GLQuad.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Util.h"
#include "Camera.h"

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line GLQuad.cpp:%d): %s\n", __LINE__, glewGetErrorString(err)); exit(-1);} }

/*Helper function to convert colors from 0-255 range to 0-1 range*/
void ConvertColor(std::vector<float>& colors) {
	unsigned int size = colors.size();
	for (unsigned int i = 0; i < size; i++) {
		colors[i] = colors[i] / 255.0;
	}
}

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(std::vector<float>& tex_coords, float tex_width, float tex_height) {
	unsigned int size = tex_coords.size();
	for (unsigned int i = 0; i < size; i += 2) {
		tex_coords[i] = tex_coords[i] / tex_width;
		tex_coords[i + 1] = tex_coords[i + 1] / tex_height;
	}
}

GLQuad::GLQuad() : Component("GLQuad"), p_texture(NULL), vao_id(0), p_owner_transform(NULL),
					texure_list_size(0), texture_mode(0) {
	tex_offset[0] = tex_offset[1] = 0;
	p_texure_list[0] = p_texure_list[1] = p_texure_list[2] =
		p_texure_list[3] = p_texure_list[4] = NULL;
}


void GLQuad::Draw(ShaderProgram* program) {
	GLuint loc = glGetUniformLocation(program->program_id, "translateMatrix");
	glm::mat4 translate_matrix = p_owner_transform->GetTranslateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(translate_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "rotateMatrix");
	glm::mat4 rotate_matrix = p_owner_transform->GetRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(rotate_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "scaleMatrix");
	glm::mat4 scale_matrix = p_owner_transform->GetScaleMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(scale_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "preRotateMatrix");
	glm::mat4 pre_rotate_matrix = p_owner_transform->GetPreRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(pre_rotate_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "postRotateMatrix");
	glm::mat4 post_rotate_matrix = p_owner_transform->GetPostRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(post_rotate_matrix));
	CHECKERROR;

	glActiveTexture(GL_TEXTURE2); // Activate texture unit 2
	glBindTexture(GL_TEXTURE_2D, p_texture->texture_id); // Load texture into it
	loc = glGetUniformLocation(program->program_id, "texture_map");
	glUniform1i(loc, 2); // Tell shader texture is in unit 2
	CHECKERROR;

	GLfloat converted_tex_offset[2];
	converted_tex_offset[0] = tex_offset[0] / p_texture->width;
	converted_tex_offset[1] = tex_offset[1] / p_texture->height;


	loc = glGetUniformLocation(program->program_id, "tex_offset");
	glUniform2fv(loc, 1, &(converted_tex_offset[0]));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "mode");
	glUniform1i(loc, texture_mode);
	CHECKERROR;

	glBindVertexArray(vao_id);
	CHECKERROR;
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

/*Links the GLQuad component with it's related components
* Related components : Transform
* Returns : void
*/
void GLQuad::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

void GLQuad::SetTexOffset(GLfloat tex_offset_x, GLfloat tex_offset_y) {
	tex_offset[0] = tex_offset_x;
	tex_offset[1] = tex_offset_y;
}

//Sets the render mode to color (int=0) or texture (int=1)
void GLQuad::SetTextureMode(int _mode) {
	texture_mode = _mode;
}

Texture* GLQuad::GetTexture() {
	return p_texture;
}

void GLQuad::AddTexture(Texture* _p_texture) {
	p_texure_list[texure_list_size] = _p_texture;
	texure_list_size++;
}

void GLQuad::SetTexture(unsigned int index) {
	p_texture = p_texure_list[index];
}

/*Serialize the GLquad object from a json input
* Expects a json dict with the following keys:
* texture_names : list of texture file names present in the resources folder
* vertex_list: A 1D list of 3*4 floats. Usually 4 Vertices.
			   Each group of 3 floats represents 1 vertexs x, y,and z values.
* color_list: A 1D list of 4*4 floats. Each group of 4 floats 
			  represents 1 vertexs r, g, b, and a color values.
* texture_list: A 1D list of 2*4 floats. Each group of 2 floats represents 
				the texture coordinates for each vertex.
* Returns : void
*/
void GLQuad::Serialize(json json_object) {
	auto texture_names = json_object["texture_names"].get<std::vector<std::string>>();
	for (auto texture_name : texture_names) {
		p_resource_manager->add_texture(texture_name);
		AddTexture(p_resource_manager->get_texture(texture_name));
	}
	SetTexture(0);
	//Create a VAO and put the ID in vao_id
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Put a vertex consisting of 3 float coordinates x,y,z into the list of all vertices
	auto vertices = json_object["vertex_list"].get<std::vector<float>>();
	vertex_count = vertices.size() / 3;

	//Create a continguous buffer for all the vertices/points
	GLuint point_buffer;
	glGenBuffers(1, &point_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//Put a color consisting of 4 float values rgba into the list of all colors 
	auto colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(colors);
	//Create another continuguous buffer for all the colors for each vertex
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//Put a texture coordinate cosisting of 2 uv float values 
	auto coord = json_object["tex_coord_list"].get<std::vector<float>>();
	//Convert coords from image space to 0..1
	ConvertTextureCoords(coord, p_texture->width, p_texture->height);
	//Create another continguous buffer for all the textures for each vertex
	GLuint tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coord.size(), &coord[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };
	//Create IBO
	GLuint indeces_buffer;
	glGenBuffers(1, &indeces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	CHECKERROR;
	glBindVertexArray(0);

	SetTextureMode(1);
}