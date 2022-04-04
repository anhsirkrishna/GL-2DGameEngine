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


class Collider;

/* A reference to the colliders being touched from all directions.
*  Needed to check if locks can be disabled back or not
*/
//struct CollidersTouching {
//	Collider* above, * beneath, * left, * right;
//};


class Collider : public Component {
private:
	bool enabled;
	Transform* p_owner_transform;
	Movement* p_owner_movement;


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

public:
	// Default ctor
	Collider();

	// Serialize method. Nothing to do for the collider component
	void Serialize(json json_object);

	// Set offsets relative to the transform pos
	void SetColliderOffsets(glm::vec4 offsets);

	// Which collider is this object on top of?
	void SetColliderOnTopOf(Collider* collider);

	// Update transform position
	void UpdateTransformPosition();

	// Updates the collider position
	void UpdateColliderPosition();

	// Getter to get Collider Box Position
	void SetColliderPosition(glm::vec4 new_col_pos);

	// Getter to get Collider Box Position
	glm::vec4 GetColliderPosition();

	//Set the offset position and dimensions
	void SetPositionOffset(glm::vec4 new_pos);

	/*Get the offset positionand dimensions
	* Returns: glm::vec4
	*/
	glm::vec4 GetPositionOffset();

	void Update();

	// Link other components
	void Link();

	/* Stores references to 
	   colliders being touched above, beneath, on the left, and right
	*/
	//CollidersTouching colliders_touching;

	/*Enable the collider
	* Returns: void
	*/
	void Enable();

	/*Disables the collider
	* Returns: void
	*/
	void Disable();

	/*Checks if collider is enabled
	* Returns: bool
	*/
	bool IsEnabled();

	/*Reset the collider
	* Returns: void
	*/
	virtual void Reset();

	/*Handles events
	* Returns: void
	*/
	virtual void HandleEvent(TimedEvent* p_event);
};

