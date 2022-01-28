#pragma once

#include "Component.h"
#include "Rigidbody.h"

class Controller : public Component {

public:

	//Default ctor
	Controller() : Component("RIGIDBODY") {}

	//Updates the controller
	void Update(float delta_time);

	void Link();

private:
	Rigidbody* p_owner_rigidbody;
};

