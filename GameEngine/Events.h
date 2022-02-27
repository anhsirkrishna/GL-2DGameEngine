/******************************************************************************/
/*!
/*File   Events.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/26/2022
/*  Events class header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

//Enum of the different types of events
enum class EventID
{
	collision,
	out_of_bounds,
	hit,
	NUM
};

static const EventID all_events[] = { 
	EventID::collision,
	EventID::out_of_bounds,
	EventID::hit,
	EventID::NUM };

class GameObject;

class TimedEvent {
private:
	double time;
public:
	const EventID event_id;
	const bool broadcast;
	GameObject* reciever_p_game_obj;
	TimedEvent(EventID _id, bool _broadcast = false, GameObject* _p_game_obj=nullptr);
	//virtual void Execute() = 0;
	virtual void SetTime(double _time);
	virtual double When() const;
	bool operator < (const TimedEvent& rhs);
	virtual ~TimedEvent();
};

/*Example of what a specific event is supposed to look like
*/
class HitEvent : public TimedEvent {
public:
	HitEvent(unsigned int _dmg, int _direction, GameObject* _p_game_obj, bool _broadcast = false);
	unsigned int hit_damage;
	int direction;
};
