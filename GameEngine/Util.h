#pragma once
#include <glm.hpp>

//Utility function to get the pointer from a glm::mat4 to send to gfx h/w
float* getMat4Pointer(glm::mat4& matrix);