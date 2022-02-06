#pragma once

#include <glm.hpp>

bool Colliding(glm::vec4 pos_0, glm::vec4 pos_1) {

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
