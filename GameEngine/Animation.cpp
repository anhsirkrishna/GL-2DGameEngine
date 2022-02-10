/******************************************************************************/
/*!
/*File   GameObjectFactory.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  GameObjectFactory implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Animation.h"
#include "GLQuad.h"
#include "GameObject.h"

/*Default ctor for animation component.
* Does not require any arguments
*/
Animation::Animation() : Component("ANIMATION"), p_owner_glquad(NULL), current_frame(0),
looping(true), interval(0), timer(0), times_played(0) {}

/*Default dtor for */
Animation::~Animation() {
	for (auto frame : frames) {
		delete frame;
	}
}

void Animation::Refresh() {
	current_frame = 0;
	times_played = 0;
	timer = 0;
}

bool Animation::Completed() {
	if (times_played > 0)
		return true;
	return false;
}

void Animation::Update() {
	timer += 1;

	if (timer >= interval) {
		timer = 0;
		//Move to the next frame of animation 
		if (times_played > 0 && looping == false)
			current_frame = frames.size() - 1;
		else
			current_frame = 
				current_frame + 1 < frames.size() ? (current_frame + 1) : 0;
		if (current_frame == 0)
			times_played += 1;
	}

	//Set the texture offset for the GLSprite component of the the owner
	p_owner_glquad->SetTexOffset(
		frames[current_frame][0], frames[current_frame][1]);
}

void Animation::Serialize(json json_object) {
	auto tex_offset_list = json_object["frames"].get<std::vector<int>>();
	GLfloat* temp_offset;
	for (unsigned int i = 0; i < tex_offset_list.size(); i += 2) {
		temp_offset = new GLfloat[2];
		temp_offset[0] = tex_offset_list[i];
		temp_offset[1] = tex_offset_list[i + 1];
		frames.push_back(temp_offset);
	}

	looping = json_object["looping"].get<bool>();
	interval = json_object["interval"].get<int>();
}

void Animation::Link() {
	p_owner_glquad =
		static_cast<GLQuad*>(GetOwner()->HasComponent("GLQUAD"));
}

//Returns the total duration of the animation in number of frames
int Animation::Duration() {
	return frames.size() * interval;
}