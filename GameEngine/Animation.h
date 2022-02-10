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
	Animation();
	~Animation();
	void Refresh();
	virtual void Update();
	virtual void Link();
	void Serialize(json json_object);
	bool Completed();
	int Duration();
};