/******************************************************************************/
/*!
/*File: Behavior.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   2/24/2022
/*  Implementation of the Behavior component
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "Behavior.h"
#include "LuaManager.h"

// default constructor
Behavior::Behavior() : Component("BEHAVIOR") {

}

// loads lua script to run on Update()
void Behavior::LoadScript()
{
	// registers functions from different sets (input/player/object/etc)
	p_lua_manager->RegInputFunctions(lua_state);
	p_lua_manager->RegPlayerFunctions(lua_state, GetOwner());

	std::string file = "..\\Resources\\Scripts\\";
	file += script_name;
	script_result = lua_state.load_file(file);
}

// runs the current script once per frame
void Behavior::Update()
{
	script_result();
}

//Serialize method. Nothing to do for Transform component.
void Behavior::Serialize(json json_object)
{
	script_name = json_object["script_filename"];
}
