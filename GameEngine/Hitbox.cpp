/******************************************************************************/
/* !
/* File   Hitbox.h
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Hitbox Component header file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#include "Hitbox.h"
#include "Hurtbox.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "EventManager.h"

// Check for AABB collisions between hitbox and hurtbox
bool AABB(glm::vec4 hitbox, glm::vec4 hurtbox) {

	if ((hitbox.x - hitbox.z) > (hurtbox.x + hurtbox.z)) {
		return false;
	}

	if ((hitbox.x + hitbox.z) < (hurtbox.x - hurtbox.z)) {
		return false;
	}

	if ((hitbox.y + hitbox.w) < (hurtbox.y - hurtbox.w)) {
		return false;
	}

	if ((hitbox.y - hitbox.w) > (hurtbox.y + hurtbox.w)) {
		return false;
	}

	return true;
}

Hitbox::Hitbox() : Component("HITBOX"), offset(0.0f), dimensions(0.0f),
					p_owner_transform(nullptr) {
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

}

void Hitbox::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
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
	if (CheckCollision(curr_position, p_collided_object)) {
		p_event_manager->QueueTimedEvent(
			new HitEvent(0, p_owner_transform->GetScaleX() * -1, p_collided_object));
		p_event_manager->QueueTimedEvent(
			new TimedEvent(EventID::impact, false, GetOwner()));
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
	for (unsigned int i = 0; i < p_game_obj_manager->game_object_list.size(); ++i) {
		curr_obj = p_game_obj_manager->game_object_list[i];

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