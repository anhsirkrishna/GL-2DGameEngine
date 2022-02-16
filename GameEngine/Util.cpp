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

/*Helper function to convert colors from 0-255 range to 0-1 range*/
void ConvertColor(std::vector<float>& colors) {
	unsigned int size = colors.size();
	for (unsigned int i = 0; i < size; i++) {
		colors[i] = colors[i] / 255.0;
	}
}

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(std::vector<float>& tex_coords, float tex_width,
	float tex_height) {
	unsigned int size = tex_coords.size();
	for (unsigned int i = 0; i < size; i += 2) {
		tex_coords[i] = tex_coords[i] / tex_width;
		tex_coords[i + 1] = tex_coords[i + 1] / tex_height;
	}
}