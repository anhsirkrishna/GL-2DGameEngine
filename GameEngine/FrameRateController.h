#pragma once
#include "SDL.h"
#include <algorithm>

class FrameRateController {
public:
	FrameRateController(unsigned int frameRate);
	~FrameRateController();
	/*Function called at the start of the game loop to record the time at the start*/
	void start_game_loop();
	/*Function called at the end of the game loop to get the loop end time and limit the framerate.*/
	void end_game_loop();
	/*Function called at the end of the game loop to limit the framerate*/
	void limit_frame_rate();
	Uint32 GetLoopStartTime();
	Uint32 GetPrevLoopDeltaTime();
private:
	float time_per_frame;
	Uint32 loop_start_time;
	Uint32 prev_loop_delta_time;
	int frame_counter;
	int timer;
};

extern FrameRateController* p_framerate_controller;