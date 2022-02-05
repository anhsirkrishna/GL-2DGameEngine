#pragma once

#include "Component.h"
#include "Rigidbody.h"

class Controller : public Component {

public:

	//Default ctor
	Controller() : Component("CONTROLLER"), p_owner_rigidbody(nullptr) {}

	//Updates the controller
	void Update();

	void Link();

private:
	Rigidbody* p_owner_rigidbody;
};

