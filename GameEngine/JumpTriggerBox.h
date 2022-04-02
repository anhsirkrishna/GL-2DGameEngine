#pragma once

#include "Component.h"
#include <glm.hpp>

class JumpTriggerBox : public Component
{
public:
	JumpTriggerBox();

	void Serialize(json json_object);
};

