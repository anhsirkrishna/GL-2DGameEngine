/******************************************************************************/
/*!
/*File   ResourceManager.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Resource Manager header file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "SDL.h"

class ShaderProgram;
class Texture;
class CharacterTextures;

class ResourceManager {
private:
	std::unordered_map<std::string, SDL_Surface*> resource_map;
	std::unordered_map<std::string, Texture*> textures_map;
	std::unordered_map<std::string, ShaderProgram*> shader_map;
	std::unordered_map<std::string, CharacterTextures*> font_map;
public:
	ResourceManager();
	~ResourceManager();
	/*Function to load the resource if it's not already loaded.*/
	void add_resource(const std::string& file_name);
	/*Function to free all the loaded resources.*/
	void free_resources();
	/*Function that returns the pointer to the resource identified by the name.
	  Returns NULL if the resources isn't loaded.*/
	SDL_Surface* get_resource(const std::string& file_name);

	//Function to add a new texture. The texture will be loaded onto H/w
	void add_texture(const std::string& file_name);
	//Returns the pointer to texture object which has access to its texture id
	Texture* get_texture(const std::string& name);

	/*Adds a shader program to the resource map.
	* Adding a shader creates a ShaderProgram instance.
	* This reads, loads and compiles the shader program.
	* Requires the .vert and .frag files to have the same starting prefix filename
	* Returns: void
	*/
	void add_shader(const std::string& file_name);
	//Returns pointer to the shader program with the name specified
	ShaderProgram* get_shader(const std::string& file_name);

	/*Adds a compute shader program to the resource map.
	* Adding a shader creates a ShaderProgram instance.
	* This reads, loads and compiles the shader program.
	* Requires the .comp file to have the same starting prefix filename
	* Returns: void
	*/
	void add_compute_shader(const std::string& file_name);

	/*Returns the CharacterTextures object created for a
	* specific font
	* Returns: CharacterTextures*
	*/
	CharacterTextures* get_font_textures(const std::string& ttf_file_name);

	/*Reads a ttf file and generates textures for the
	* first 128 ASCII characters according to the
	* font type
	* Returns: void
	*/
	void add_font_textures(const std::string& ttf_file_name);
};

extern ResourceManager * p_resource_manager;
