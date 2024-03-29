/******************************************************************************/
/*!
/*File: Behavior.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   2/24/2022
/*  Implementation of the Behavior component
*
/* DigiPen Institute of Technology � 2022
/******************************************************************************/
#include "Behavior.h"
#include "LuaManager.h"
#include "Events.h"
#include "EventManager.h"
#include <sstream>
#include <fstream>

// default constructor
Behavior::Behavior() : Component("BEHAVIOR"), script_result() {

}

// loads lua script to run on Update()
void Behavior::LoadScript() {
	// registers functions from different sets (input/player/object/etc)
	p_lua_manager->RegGlobals(lua_state);
	p_lua_manager->RegObjectFunctions(lua_state, GetOwner());
	p_lua_manager->RegEvents(lua_state, nullptr);
	std::string file = "..\\Resources\\Scripts\\";
	file += script_name;
	script_result = lua_state.load_file(file);

	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();
	script_string = buffer.str();
	t.close();
	
}

// runs the current script once per frame
void Behavior::Update()
{
	p_lua_manager->UpdateLuaState(lua_state);
	script_result();
	p_lua_manager->RegEvents(lua_state, nullptr);
}

//Serialize method. Nothing to do for Transform component.
void Behavior::Serialize(json json_object) {
	script_name = json_object["script_filename"];
}

void Behavior::Link() {
	LoadScript();
}

/*Sends the received event to the lua script
* Returns : void
*/
void Behavior::HandleEvent(TimedEvent* p_event) {
	p_lua_manager->RegEvents(lua_state, p_event);
}

/*Send an event using the event manager
* Allows lua scripts to send events
* Args need to be primitive types for lua to send
* Returns: void
*/
void Behavior::SendEvent(int event_id, int delay_time, bool broadcast) {
	if (broadcast) {
		p_event_manager->QueueTimedEvent(
			new TimedEvent(static_cast<EventID>(event_id), broadcast, nullptr, delay_time)
		);
	}
	else {
		p_event_manager->QueueTimedEvent(
			new TimedEvent(static_cast<EventID>(event_id), broadcast, GetOwner(), delay_time)
		);
	}
}

void Behavior::SetScriptName(std::string script_name_) {
	script_name = script_name_;
}

std::string Behavior::GetScriptName()
{
	return script_name;
}

std::string Behavior::GetScriptString()
{
	return script_string;
}