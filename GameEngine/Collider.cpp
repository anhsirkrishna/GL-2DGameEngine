#include "Collider.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Movement.h"
#include "Util.h"

#include <iostream>

Collider::Collider() : Component("COLLIDER"), 
					   p_owner_transform(nullptr), 
					   p_owner_movement(nullptr),
					   pos_offset(glm::vec4(0)),
					   col_pos(glm::vec4(0)),
					   enabled(true)
{}

// Serializing Pos offset from JSON
void Collider::Serialize(json json_object) {
	auto col_offset_params = json_object["collider_offset_params"].get<std::vector<int>>();
	pos_offset = glm::vec4(col_offset_params[0],
		col_offset_params[1],
		col_offset_params[2],
		col_offset_params[3]);
}

// Set collider offset values
void Collider::SetColliderOffsets(glm::vec4 offsets)
{
	pos_offset = offsets;
}


/* Update transform position after 
 * updating collider positions
 */
void Collider::UpdateTransformPosition()
{
	glm::vec4 new_pos = p_owner_transform->GetPosition();

	new_pos.x = col_pos.x - pos_offset.x - pos_offset.z / 2.0f;
	new_pos.y = col_pos.y - pos_offset.y - pos_offset.w / 2.0f;

	p_owner_transform->SetPosition(new_pos);
}


/* Update collider position
 * values are relative to the Transform position
 */
void Collider::UpdateColliderPosition()
{
	glm::vec4 pos = p_owner_transform->GetPosition();
	
	col_pos.x = pos.x + pos_offset.x + pos_offset.z / 2.0f;
	col_pos.y = pos.y + pos_offset.y + pos_offset.w / 2.0f;
	col_pos.z = pos_offset.z / 2.0f;
	col_pos.w = pos_offset.w / 2.0f;
}

void Collider::SetColliderPosition(glm::vec4 new_col_pos)
{
	col_pos = new_col_pos;
}


// Get collider position
glm::vec4 Collider::GetColliderPosition()
{
	return col_pos;
}

// Update function for Collider
void Collider::Update()
{
	UpdateColliderPosition();
}

// Link other components to this one
void Collider::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}


void Collider::Enable() {
	enabled = true;
}

void Collider::Disable() {
	enabled = false;
}

bool Collider::IsEnabled() {
	return enabled;
}

void Collider::Reset() {
	enabled = true;
}
