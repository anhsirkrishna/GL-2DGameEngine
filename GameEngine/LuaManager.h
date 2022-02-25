#pragma once
/******************************************************************************/
/*!
/*File: LuaManager.h
/*Author: Brady Menendez
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

// Custom FMOD Wrapper Class
class LuaManager
{
	sol::state lua_state;

public:

	LuaManager();
	~LuaManager();
	void Update();
	void RegPlayerFunctions(sol::state& state, GameObject* obj);
	void RegInputFunctions(sol::state& state);

	void LoadBehaviorScripts();

	void Cleanup();
};

//Declared as an extern variable so it can be accessed throughout the project
extern LuaManager* p_lua_manager;

