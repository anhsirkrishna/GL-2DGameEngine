/******************************************************************************/
/*!
/*File   CharacterTextures.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/* Character Texture class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include <string>
#include <map>
#include <glm.hpp>

typedef unsigned int GLuint;

struct Character {
	GLuint texture_id;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class CharacterTextures {	
public:
	glm::vec2 texture_dimensions;
	std::map<char, Character> character_map;
	/*Creates a map of characters and their associated textures
	* that can be used to render a character using the font type
	* specified in the argument
	*/
	CharacterTextures(std::string ttf_filepath);
};