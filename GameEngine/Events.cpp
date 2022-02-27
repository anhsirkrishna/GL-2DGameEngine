#include "Events.h"
#include "FrameRateController.h"
#include "GameObject.h"

TimedEvent::TimedEvent(EventID _id, bool _broadcast, GameObject* _p_game_obj) : 
	time(0), event_id(_id), broadcast(_broadcast), 
	reciever_p_game_obj(_p_game_obj) { }

TimedEvent::~TimedEvent() {

}

//Set the time the event should be trigger. 
//Time is specified as number of milliseconds from the time of calling this function
void TimedEvent::SetTime(double _time) {
	time = p_framerate_controller->GetLoopStartTime() + _time;
}

//Returns the number of milliseconds until the event needs to be triggered
//Event needs to be triggered when the returned value is 0 or less
double TimedEvent::When() const {
	return  time - p_framerate_controller->GetLoopStartTime();
}

//If two events are compared to see which one needs to be executed quicker
bool TimedEvent::operator<(const TimedEvent& rhs) {
	return time < rhs.time;
}


HitEvent::HitEvent(unsigned int _dmg, int _direction, 
				   GameObject* _p_game_obj, bool _broadcast) : 
	TimedEvent(EventID::hit, _broadcast, _p_game_obj), 
	hit_damage(_dmg), direction(_direction) {}
