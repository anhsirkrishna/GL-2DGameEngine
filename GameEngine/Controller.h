#pragma once

/******************************************************************************/
/* !
/* File:   Controller.h
/* Author: Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date:   01/27/2022
/* Controller Component class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Component.h"
#include "Rigidbody.h"

class Controller : public Component {

public:

	// Default ctor
	Controller() : Component("CONTROLLER"), p_owner_rigidbody(nullptr) {}

	// Updates the controller
	void Update();

	// Stores references to other components
	void Link();

private:
	Rigidbody* p_owner_rigidbody;
};

