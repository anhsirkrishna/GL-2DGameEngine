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

// LevelManager class
class LevelManager
{
public:
	int current_level;

	LevelManager() : current_level(0) {};
	void LoadLevel(int level);
	void ReloadLevel();
};

//Declared as an extern variable so it can be accessed throughout the project
extern LevelManager* p_level_manager;