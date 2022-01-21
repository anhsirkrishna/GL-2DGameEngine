#pragma once

#include "Component.h"

class Controller : public Component {

public:

	//Default ctor
	Controller() : Component("RIGIDBODY") {}

	//Updates the controller
	void Update(float delta_time);
};

