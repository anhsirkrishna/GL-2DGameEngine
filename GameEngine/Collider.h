/******************************************************************************/
/* !
/* File:   Collider.h
/* Author: Srey Raychaudhuri
/* Email:  srey.raychaudhuri@digipen.edu
/* Date:   02/06/2022
/* Collider Component class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Transform.h"
#include "Rigidbody.h"

class Collider : public Component {
public:
	// Default ctor
	Collider();

	/*
	* Update called once every game loop for the collider component
	* Checks for collisions with every other game object.
	*/
	void Update();

	// Link other components
	void Link();

private:
	Transform* p_owner_transform;
	Rigidbody* p_owner_rigidbody;
};

