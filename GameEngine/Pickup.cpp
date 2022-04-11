/******************************************************************************/
/* !
/* File   Pickup.cpp
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Pickup Component header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Pickup.h"
#include "Transform.h"
#include "GameObject.h"
#include "Events.h"
#include "DependantObjects.h"
#include "EventManager.h"
#include "Hurtbox.h"

Pickup::Pickup() : Component("PICKUP"), offset(0.0f), dimensions(0.0f),
					enabled(false), p_owner_transform(nullptr), 
					p_owner_dep_obj(nullptr), p_pickup_character(nullptr), 
					p_pickup_character_hurtbox(nullptr) {
}

/*Update the component
* moves to the offset
* checks owner tranform scale
* checks collisions with player character
* sends pickup events
*/
void Pickup::Serialize(json json_object) {
	//Offset from the owner transform
	auto offset_vals = json_object["offset"].get<std::vector<float>>();
	offset.x = offset_vals[0];
	offset.y = offset_vals[1];

	//The height and width of the hitbox
	auto dimension_vals = json_object["dimensions"].get<std::vector<float>>();
	dimensions.x = dimension_vals[0];
	dimensions.y = dimension_vals[1];

	if (json_object.contains("enabled")) {
		enabled = json_object["enabled"].get<bool>();
	}
}
void Pickup::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_dep_obj = static_cast<DependantObjects*>(GetOwner()->HasComponent("DEPENDANT_OBJECTS"));
	p_pickup_character = p_owner_dep_obj->GetDependtantObjectPointer(0);
	p_pickup_character_hurtbox = static_cast<Hurtbox*>(p_pickup_character->HasComponent("HURTBOX"));
}


void Pickup::Reset() {
	enabled = true;
}


void Pickup::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id)
	{
	case EventID::enable:
		enabled = true;
		break;
	case EventID::disable:
		enabled = false;
		break;
	}
}

/*Update the component
* moves to the offset
* checks owner tranform scale
* checks collisions with hurtboxes
* sends hit events
*/
void Pickup::Update() {
	glm::vec4 curr_position;
	glm::vec4 owner_pos = p_owner_transform->GetPosition();
	curr_position.x = owner_pos.x + (offset.x * p_owner_transform->GetScaleX());
	curr_position.y = owner_pos.y + (offset.y * p_owner_transform->GetScaleY());
	curr_position.z = dimensions.x;
	curr_position.w = dimensions.y;
	
	if (enabled) {
		if (CheckCollision(curr_position)) {
			p_event_manager->QueueTimedEvent(
				new TimedEvent(EventID::picked_up, false, GetOwner()));
			p_event_manager->QueueTimedEvent(
				new TimedEvent(EventID::pickup, false, p_pickup_character));
			enabled = false;
		}
	}
}

/*Checks collision between the hitbox and all other hurtboxes
* Returns: true if collision.
		   Stores the collided object in second argument
*/
bool Pickup::CheckCollision(glm::vec4 pickup_box) {
	glm::vec4 hurtbox_pos = p_pickup_character_hurtbox->GetHurtboxPos();

	if ((pickup_box.x) > (hurtbox_pos.x + hurtbox_pos.z)) {
		return false;
	}

	if ((pickup_box.x + pickup_box.z) < (hurtbox_pos.x)) {
		return false;
	}

	if ((pickup_box.y + pickup_box.w) < (hurtbox_pos.y)) {
		return false;
	}

	if ((pickup_box.y) > (hurtbox_pos.y + hurtbox_pos.w)) {
		return false;
	}

	return true;
}