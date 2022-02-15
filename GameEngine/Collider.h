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
#include <glm.hpp>


// Forward Declaration
class Transform;
class Movement;


class Collider : public Component {
public:
	// Default ctor
	Collider();

	// Serialize method. Nothing to do for the collider component
	void Serialize(json json_object);

	// Set offsets relative to the transform pos
	void SetColliderOffsets(glm::vec4 offsets);

	// Update transform position
	void UpdateTransformPosition();

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

	/* A reference to the collider being touched.
	*  Needed to check if gravity can be enabled back or not
	*  Depending on whether one collider is 
	*  left or right of collider_touching
	*/
	Collider* collider_touching;

	/* Offset relative to transform component
	 * x : offset from transform pos along x-axis
	 * y : offset from transform pos along y-axis
	 * z : width of collider box
	 * w : height of collider box
	 */
	glm::vec4 pos_offset;

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

