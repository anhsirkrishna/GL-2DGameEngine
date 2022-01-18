#include "Util.h"

float* getMat4Pointer(glm::mat4& matrix) {
	return &(matrix[0][0]);
}

std::string Mat4PrintableStr(glm::mat4 const& matrix) {
	std::string ret_string = "";
	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			ret_string += std::to_string(matrix[j][i]);
			ret_string += ", \t ";
		}
		ret_string += "\n";
	}
	return ret_string;
}