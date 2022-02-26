/******************************************************************************/
/*!
/*File   EventManager.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/26/2022
/*  Event Management header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include <vector>
#include <unordered_map>
#include "Events.h"

class GameObject;

class EventManager {
private:
	std::vector<TimedEvent*> timed_event_list;
	std::vector<TimedEvent*> new_events;
	std::unordered_map<EventID, std::vector<GameObject*>> subscriber_list;
	void BroadcastEvent(TimedEvent* p_event);
	void BroadcastEventToSubscribers(TimedEvent* p_event);
	void SendEvent(TimedEvent* p_event, GameObject* p_game_object);
public:
	EventManager();
	void QueueTimedEvent(TimedEvent* p_event);
	void Update();
	void SubscribeToEvent(EventID event_type, GameObject* p_game_object);
	void ClearSubscribeList();
};

extern EventManager* p_event_manager;