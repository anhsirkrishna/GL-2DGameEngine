#include "Hurtbox.h"
#include "Transform.h"
#include "GameObject.h"
#include "Events.h"

Hurtbox::Hurtbox() : Component("HURTBOX"), offset(0.0f), dimensions(0.0f),
					 p_owner_transform(nullptr), enabled(true) {
}

/*Inits a Hurtbox component
* Expects a dict with the following key:values
* offset : float[2]
* dimensions : float[2]
* Returns : void
*/
void Hurtbox::Serialize(json json_object) {
	//Offset from the owner transform
	auto offset_vals = json_object["offset"].get<std::vector<float>>();
	offset.x = offset_vals[0];
	offset.y = offset_vals[1];

	//The height and width of the Hurtbox
	auto dimension_vals = json_object["dimensions"].get<std::vector<float>>();
	dimensions.x = dimension_vals[0];
	dimensions.y = dimension_vals[1];

}

void Hurtbox::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

/*Update the component
* moves to the offset
* checks owner tranform scale
* checks collisions with hurtboxes
* sends hit events
*/
void Hurtbox::Update() {
}

/*Returns a vec4 containing the position,
* width and height
* Returns : vec4
*/
glm::vec4 Hurtbox::GetHurtboxPos() {
	glm::vec4 curr_position;
	glm::vec4 owner_pos = p_owner_transform->GetPosition();
	curr_position.x = owner_pos.x + (offset.x);
	curr_position.y = owner_pos.y + (offset.y);
	curr_position.z = dimensions.x;
	curr_position.w = dimensions.y;

	return curr_position;
}

/*Handles events
* Returns: void
*/
void Hurtbox::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id)
	{
	case EventID::die:
		enabled = false;
		break;
	case EventID::disable:
		enabled = false;
		break;
	default:
		break;
	}
}

void Hurtbox::Reset() {
	enabled = true;
}

//Returns enabled state
bool Hurtbox::IsEnabled() {
	return enabled;
}
