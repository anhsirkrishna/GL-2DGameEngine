/******************************************************************************/
/* !
/* File:   Collider.h
/* Author: Srey Raychaudhuri
/* Email:  srey.raychaudhuri@digipen.edu
/* Date:   02/06/2022
/* Collider Component class encapsulation header file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#pragma once

#include "Component.h"
#include <nlohmann/json.hpp>

// Forward Declaration
class Transform;
class Movement;

using json = nlohmann::json;

class Collider : public Component {
public:
	// Default ctor
	Collider();

	// Serialize method. Nothing to do for the collider component
	void Serialize(json json_object);

	/*
	* Update called once every game loop for the collider component
	* Checks for collisions with every other game object.
	*/
	void Update();

	// Link other components
void Link();

private:
	Transform* p_owner_transform;
	Movement* p_owner_movement;
};

