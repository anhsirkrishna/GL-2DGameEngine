#pragma once
#include "Component.h"
#include "Transform.h"
#include "Events.h"

#include <glm.hpp>

class JumpTriggerReceiverBox : public Component
{
private:
	glm::vec2 offset;
	glm::vec2 dimensions;
	Transform* p_owner_transform;
	bool enabled;

	// After box is disabled, when timer goes down to 0, box is enabled again 
	float reenable_timer;

public:
	JumpTriggerReceiverBox();

	// Update function
	void Update();

	void Serialize(json json_object);

	virtual void HandleEvent(TimedEvent* pEvent);

	void Link();

	bool isEnabled();
};

