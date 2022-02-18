/******************************************************************************/
/*!
/*File   Animation.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Animation component implementation file
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

//Default dtor for Animations
Animation::~Animation() {
	for (auto frame : frames) {
		delete frame;
	}
}

// Refreshes the animation to the start
void Animation::Refresh() {
	current_frame = 0;
	times_played = 0;
	timer = 0;
}

/*Function to check if one complete cycle of animation has finished
* Returns: bool - True if one cycle is complete
*/
bool Animation::Completed() {
	if (times_played > 0)
		return true;
	return false;
}

/*Animation component Update call
* Modifies the parent game object's GLQuad component
* by setting the texture offset to the value that 
* corresponds to the current frame of animation
*/
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

/*Creates the animation component from the json object
* Expects three keys :
* frames - List of (x,y) values that specify the texture offset for each
		   frame of animation
* loop - Boolean that determines if the animation loops 
* interval - The number of frames each frame of animation will last for.
*/
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

//Links other related components
void Animation::Link() {
	p_owner_glquad =
		static_cast<GLQuad*>(GetOwner()->HasComponent("GLQUAD"));
}

//Returns the total duration of the animation in number of frames
int Animation::Duration() {
	return (frames.size()/2) * interval;
}

int Animation::GetCurrentFrame()
{
	return current_frame;
}