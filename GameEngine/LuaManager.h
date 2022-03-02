/******************************************************************************/
/*!
/*File: LuaManager.h
/*Author: Brady Menendez
*		  Krishna Pillai - Added logging
/*Email: brady.m@digipen.edu
/*Date   2/24/2022
/*  Definition of the Lua Manager Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once
#include <sol/sol.hpp>
#include <string>

class GameObject;
class TimedEvent;

// Luabinding wrapper class using SOL
class LuaManager
{
	sol::state lua_state;

public:

	LuaManager();
	~LuaManager();
	void Update();
	void RegObjectFunctions(sol::state& state, GameObject* obj);
	void RegGlobals(sol::state& state);
	void RegEvents(sol::state& state, TimedEvent* p_event);
	void LoadBehaviorScripts();
	void Cleanup();

	/*Call SDL_Log
	* accessed from inside Lua script
	* Returns: void
	*/
	void LogMessage(std::string log_str);
};

//Declared as an extern variable so it can be accessed throughout the project
extern LuaManager* p_lua_manager;

