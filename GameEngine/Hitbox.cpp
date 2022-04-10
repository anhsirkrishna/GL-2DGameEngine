/******************************************************************************/
/* !
/* File   Hitbox.h
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Hitbox Component header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Hitbox.h"
#include "Hurtbox.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include "DependantObjects.h"

// Check for AABB collisions between hitbox and hurtbox
bool AABB(glm::vec4 hitbox, glm::vec4 hurtbox) {

	if ((hitbox.x) > (hurtbox.x + hurtbox.z)) {
		return false;
	}

	if ((hitbox.x + hitbox.z) < (hurtbox.x)) {
		return false;
	}

	if ((hitbox.y + hitbox.w) < (hurtbox.y)) {
		return false;
	}

	if ((hitbox.y) > (hurtbox.y + hurtbox.w)) {
		return false;
	}

	return true;
}

Hitbox::Hitbox() : Component("HITBOX"), offset(0.0f), dimensions(0.0f),
					p_owner_transform(nullptr), enabled(false), timer_till_enabled(0) {
}

/*Inits a hitbox component
* Expects a dict with the following key:values
* offset : float[2]
* dimensions : float[2]
* Returns : void
*/
void Hitbox::Serialize(json json_object) {
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


void Hitbox::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}


void Hitbox::Reset()
{
	enabled = false;
	p_event_manager->QueueTimedEvent(
		new TimedEvent(EventID::enable, false, GetOwner(), 300.0f));
}


void Hitbox::HandleEvent(TimedEvent* p_event)
{
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
void Hitbox::Update() {
	glm::vec4 curr_position;
	glm::vec4 owner_pos = p_owner_transform->GetPosition();
	curr_position.x = owner_pos.x + (offset.x * p_owner_transform->GetScaleX());
	curr_position.y = owner_pos.y + (offset.y * p_owner_transform->GetScaleY());
	curr_position.z = dimensions.x;
	curr_position.w = dimensions.y;
	GameObject* p_collided_object = nullptr;
	
	if (enabled) {
		if (CheckCollision(curr_position, p_collided_object)) {
			p_event_manager->QueueTimedEvent(
				new HitEvent(0, p_owner_transform->GetScaleX() * -1, p_collided_object));
			p_event_manager->QueueTimedEvent(
				new TimedEvent(EventID::impact, false, GetOwner()));
		}
	}
}



/*Checks collision between the hitbox and all other hurtboxes
* Returns: true if collision. 
		   Stores the collided object in second argument
*/
bool Hitbox::CheckCollision(glm::vec4 curr_position, GameObject*& collided_object) {
	GameObject* owner_obj = GetOwner();
	GameObject* curr_obj;
	Hurtbox* hurtbox;
	glm::vec4 hurtbox_pos;
	DependantObjects* p_dependants = static_cast<DependantObjects*>(GetOwner()->HasComponent("DEPENDANT_OBJECTS"));
	for (unsigned int i = 0; i < p_game_obj_manager->game_object_list.size(); ++i) {
		curr_obj = p_game_obj_manager->game_object_list[i];
		
		if (p_dependants != NULL && p_dependants->IsObjectDependant(curr_obj))
			continue;
		hurtbox = static_cast<Hurtbox*>(curr_obj->HasComponent("HURTBOX"));
		if (hurtbox == nullptr)
			continue;

		if (!hurtbox->IsEnabled())
			continue;

		if (owner_obj == curr_obj)
			continue;

		hurtbox_pos = hurtbox->GetHurtboxPos();
		if (AABB(curr_position, hurtbox_pos)) {
			collided_object = curr_obj;
			return true;
		}
	}
	return false;
}