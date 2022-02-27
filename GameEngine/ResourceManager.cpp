/******************************************************************************/
/*!
/*File   ResourceManager.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Resource Manager implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "MemoryManager.h"

#include "SDL_image.h"
#include <GL/glew.h>

ResourceManager::ResourceManager() {

}
ResourceManager::~ResourceManager() {
	free_resources();
}

void ResourceManager::add_resource(std::string file_name) {
	/*Statically assigning the resource folder path.
	  Could potentially change this if we expect to get resources from other locations*/
	std::string file_path = "..\\Resources\\";
	std::string extension = ".bmp";

	file_path += file_name;
	file_path += extension;

	//Checking if resource exists before re-loading it.
	if (get_resource(file_name) == NULL)
		resource_map.insert({ file_name, SDL_LoadBMP(file_path.c_str()) });
}

void ResourceManager::free_resources() {
	for (auto resource : resource_map)
		SDL_FreeSurface(resource.second);

	for (auto texture : textures_map)
		delete texture.second;

	for (auto shader : shader_map)
		delete shader.second;
}

SDL_Surface* ResourceManager::get_resource(std::string file_name) {
	try {
		return resource_map.at(file_name);
	}// .at(key) throws an out_of_range exception if the key doesn't exist in the map
	catch (const std::out_of_range& oor) {
		return NULL;
	};
}

//Function to add a new texture. The texture will be loaded onto H/w
void ResourceManager::add_texture(std::string file_name) {
	/*Statically assigning the resource folder path.
	  Could potentially change this if we expect to get textures from other locations*/
	std::string file_path = "..\\Resources\\";
	//All textures will be in png format
	std::string extension = ".png";

	file_path += file_name;
	file_path += extension;

	//Checking if Texture exists before re-loading it.
	if (get_texture(file_name) == NULL)
		textures_map.insert({ file_name, new Texture(file_path) });
}

//Returns the pointer to texture object which has access to its texture id
Texture* ResourceManager::get_texture(std::string name) {
	try {
		return textures_map.at(name);
	}// .at(key) throws an out_of_range exception if the key doesn't exist in the map
	catch (const std::out_of_range& oor) {
		return NULL;
	};
}

//Returns pointer to the shader program with the name specified
ShaderProgram* ResourceManager::get_shader(std::string name) {
	try {
		return shader_map.at(name);
	}
	catch (const std::out_of_range& oor) {
		return NULL;
	}
}

/*Adds a shader program to the resource map.
* Adding a shader creates a ShaderProgram instance.
* This reads, loads and compiles the shader program.
* Requires the .vert and .frag files to have the same starting prefix filename
* Returns: void
*/
void ResourceManager::add_shader(std::string file_name) {
	if (get_shader(file_name) == NULL) {
		std::string vertex_shader_name = file_name + ".vert";
		std::string frag_shader_name = file_name + ".frag";
		ShaderProgram* p_shader_program = new ShaderProgram();
		p_shader_program->AddShader(vertex_shader_name.c_str(), GL_VERTEX_SHADER);
		p_shader_program->AddShader(frag_shader_name.c_str(), GL_FRAGMENT_SHADER);
		p_shader_program->LinkProgram();
		shader_map.insert({ file_name, p_shader_program });
	}
}

/*Adds a compute shader program to the resource map.
* Adding a shader creates a ShaderProgram instance.
* This reads, loads and compiles the shader program.
* Requires the .comp file to have the same starting prefix filename
* Returns: void
*/
void ResourceManager::add_compute_shader(std::string file_name) {
	if (get_shader(file_name) == NULL) {
		std::string compute_shader_name = file_name + ".comp";
		ShaderProgram* p_shader_program = new ShaderProgram();
		p_shader_program->AddShader(compute_shader_name.c_str(), GL_COMPUTE_SHADER);
		p_shader_program->LinkProgram();
		shader_map.insert({ file_name, p_shader_program });
	}
}