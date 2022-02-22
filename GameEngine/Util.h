#pragma once
#include <glm.hpp>
#include <string>
#include <vector>

//Utility function to get the pointer from a glm::mat4 to send to gfx h/w
float const* getMat4Pointer(glm::mat4 const& matrix);

/*
* Returns the Matrix in a readable string for debug purposes
* Returns std::string : The matrix in readable format
*/
std::string Mat4PrintableStr(glm::mat4 const &matrix);


/*Helper function to convert colors from 0-255 range to 0-1 range*/
void ConvertColor(std::vector<float>& colors);

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(std::vector<float>& tex_coords, float tex_width,
	float tex_height);