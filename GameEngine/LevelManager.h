/******************************************************************************/
/*!
/*File: LevelManager.h
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   3/1/2022
/*  Definition of the LevelManager Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// LevelManager class
class LevelManager
{
public:
	int current_level;
	std::unordered_map<std::string, json> curr_obj_map;

	LevelManager() : current_level(0) {};
	void LoadLevel(int level);
	void ReloadLevel();
	void SaveLevel();
	void SaveSingle();
};

//Declared as an extern variable so it can be accessed throughout the project
extern LevelManager* p_level_manager;