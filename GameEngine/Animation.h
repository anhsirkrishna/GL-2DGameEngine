/******************************************************************************/
/*!
/*File   GameObjectFactory.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  GameObjectFactory implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "Component.h"

#include "GL/glew.h"
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class GLQuad;

class Animation : public Component {
private:
	//The list of texture offsets to apply on owner_glquad
	std::vector<GLfloat*> frames;
	//Pointer to owner glsprite
	GLQuad* p_owner_glquad;
	//Index of current frame of animation
	unsigned int current_frame;
	//Bool to determine if it's a looping animation or not
	bool looping;
	//Time interval that determines how many frames each frame of animation stays for.
	int interval;
	//How many frames that the current frame of animation has completed
	int timer;
	//Number of loops of complete animations
	unsigned int times_played;
public:
	/*Default ctor for animation component.
	* Does not require any arguments
	*/
	Animation();
	//Default dtor for Animations
	~Animation();
	// Refreshes the animation to the start
	void Refresh();
	/*Animation component Update call
	* Modifies the parent game object's GLQuad component
	* by setting the texture offset to the value that
	* corresponds to the current frame of animation
	*/
	virtual void Update();
	//Links other related components
	virtual void Link();
	/*Creates the animation component from the json object
	* Expects three keys :
	* frames - List of (x,y) values that specify the texture offset for each
			   frame of animation
	* loop - Boolean that determines if the animation loops
	* interval - The number of frames each frame of animation will last for.
	*/
	void Serialize(json json_object);
	/*Function to check if one complete cycle of animation has finished
	* Returns: bool - True if one cycle is complete
	*/
	bool Completed();
	//Returns the total duration of the animation in number of frames
	int Duration();

	/*Change the state of the Animation component
	* Involves changing the texture offsets for animation
	* Returns : void
	*/
	void ChangeState(json json_object);
};