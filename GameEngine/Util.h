#pragma once
#include <glm.hpp>
#include <string>

//Utility function to get the pointer from a glm::mat4 to send to gfx h/w
float* getMat4Pointer(glm::mat4& matrix);

/*
* Returns the Matrix in a readable string for debug purposes
* Returns std::string : The matrix in readable format
*/
std::string Mat4PrintableStr(glm::mat4 const &matrix);