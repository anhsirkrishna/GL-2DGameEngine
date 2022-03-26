#include "JumpTriggerReceiverBox.h"
#include "JumpTriggerBox.h"
#include "GameObjectManager.h"
#include "EventManager.h"

JumpTriggerReceiverBox::JumpTriggerReceiverBox() : Component("JUMP_TRIGGER_RECEIVER_BOX"), 
	offset(glm::vec2(0)), dimensions(glm::vec2(0)), enabled(true), p_owner_transform(nullptr), reenable_timer(0)
{
}

bool AABB(glm::vec4& pos_0, glm::vec4& pos_1) {

	if ((pos_0.x - pos_0.z) > (pos_1.x + pos_1.z)) {
		return false;
	}

	if ((pos_0.x + pos_0.z) < (pos_1.x - pos_1.z)) {
		return false;
	}

	if ((pos_0.y + pos_0.w) < (pos_1.y - pos_1.w)) {
		return false;
	}

	if ((pos_0.y - pos_0.w) > (pos_1.y + pos_1.w)) {
		return false;
	}

	return true;
}

void JumpTriggerReceiverBox::Update() {

	// Timer countdown in case box is disabled
	if (reenable_timer > 0) {
		reenable_timer -= 1.0f;
	}
	else {
		// if timer <= 0, reenable collision detection
		enabled = true;
	}

	glm::vec4 curr_position;
	glm::vec4 owner_pos = p_owner_transform->GetPosition();
	curr_position.x = owner_pos.x + (offset.x * p_owner_transform->GetScaleX());
	curr_position.y = owner_pos.y + (offset.y * p_owner_transform->GetScaleY());
	curr_position.z = dimensions.x;
	curr_position.w = dimensions.y;
	
	auto go_list = p_game_obj_manager->game_object_list;

	if (enabled) {
		for each (GameObject * g in go_list) {

			// first check if the other GO has a JumpTriggerBox and that it is not the owner GO itself. 
			if (g->HasComponent("JUMP_TRIGGER_BOX") && GetOwner() != g) {
				auto jump_trig_box_pos = static_cast<Transform*>(g->HasComponent("TRANSFORM"))->GetPosition();
				//auto jump_trig_box_pos = static_cast<JumpTriggerBox*>(g->HasComponent("JUMP_TRIGGER_BOX"))->GetJumpTrigBoxPos();
				if (AABB(curr_position, jump_trig_box_pos)) {
					p_event_manager->QueueTimedEvent(new TimedEvent(EventID::jump, false, GetOwner()));
				}
			}
		}
	}


}

void JumpTriggerReceiverBox::Serialize(json json_object)
{
	auto offset_vals = json_object["offset"].get<std::vector<float>>();
	offset.x = offset_vals[0];
	offset.y = offset_vals[1];

	//The height and width of the Jump Trigger Receiver Box
	auto dimension_vals = json_object["dimensions"].get<std::vector<float>>();
	dimensions.x = dimension_vals[0];
	dimensions.y = dimension_vals[1];
}

void JumpTriggerReceiverBox::HandleEvent(TimedEvent* p_event)
{
	switch (p_event->event_id)
	{
		case EventID::jump:
			enabled = false;
			reenable_timer = 120.0;
			break;
		default:
			break;
	}
}

void JumpTriggerReceiverBox::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

bool JumpTriggerReceiverBox::isEnabled()
{
	return enabled;
}
