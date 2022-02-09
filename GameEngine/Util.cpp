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


bool AABB(glm::vec4 pos_0, glm::vec4 pos_1) {

	if (pos_0.x > (pos_1.x + pos_1.z)) {
		return false;
	}

	if ((pos_0.x + pos_0.z) < pos_1.x) {
		return false;
	}

	if (pos_0.y > (pos_1.y + pos_1.w)) {
		return false;
	}

	if ((pos_0.y + pos_0.w) < pos_1.y) {
		return false;
	}

	return true;
}