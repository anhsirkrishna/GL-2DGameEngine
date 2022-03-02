/******************************************************************************/
/*!
/*File: Behavior.h
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   2/24/2022
/*  Definition of the Behavior component
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once

#include "Component.h"
#include <sol/sol.hpp>

class Behavior : public Component {

	std::string script_name;
	sol::state lua_state;
	sol::load_result script_result;

public:

	// default constructor
	Behavior();

	// loads lua script to run on Update()
	void LoadScript();

	// runs the current script once per frame
	void Update();

	//Serialize method. Nothing to do for Transform component.
	void Serialize(json json_object);

	virtual void Link();

	/*Sends the received event to the lua script
	* Returns : void
	*/
	virtual void HandleEvent(TimedEvent* p_event);
};