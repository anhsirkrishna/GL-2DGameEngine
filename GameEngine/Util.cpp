#include "Util.h"

float* getMat4Pointer(glm::mat4& matrix) {
	return &(matrix[0][0]);
}