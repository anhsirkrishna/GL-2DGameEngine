/******************************************************************************/
/*!
/*File   FadeOutState.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  BaseState header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "BaseState.h"
#include <vector>

class GameObject;
class FadeScreen;

class FadeOutState : public BaseState {
private:
	GameObject* fade_screen;
	FadeScreen* fade_screen_comp;
public:
	/*Initialize the play state
	* Create the level and all the objects
	* within the level
	*/
	FadeOutState();
	/*Deletes the play state by
	* clearing the level and cleaning
	* up all the objects
	*/
	virtual ~FadeOutState();
	/*Enters the state from another state
	* Doesn't need any special action
	* Returns: void
	*/
	virtual void Update();
	/*Calls the render/draw call for all the objects
	* Returns: void
	*/
	virtual void Render();
};