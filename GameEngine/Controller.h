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
#include "Movement.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Controller : public Component {

public:

	// Default ctor
	Controller() : Component("CONTROLLER"), p_owner_movement(nullptr) {}

	// Updates the controller
	void Update();

	// Serializer. Does nothing for this component
	void Serialize(json json_object);

	// Stores references to other components
	void Link();

private:
	Movement* p_owner_movement;
};

