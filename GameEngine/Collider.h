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

#include <nlohmann/json.hpp>
#include <glm.hpp>

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

	// Updates the collider position
	void UpdateColliderPosition();

	// Getter to get Collider Box Position
	glm::vec4 GetColliderPosition();

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


	/* Stores the position of the collider.
	 * This position = center of the collider box 
	 * x : x-coordinate of pos
	 * y : y-coordinate of pos
	 * z : half-width of collider box
	 * w : half-height of collider box
	 */
	glm::vec4 col_pos;
	
	// Check for AABB collisions
	bool AABB(glm::vec4 pos_0, glm::vec4 pos_1);
};

