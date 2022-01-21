#include "FrameRateController.h"
#include "Util.h"
#include <string>

/*Specify the framerate in FPS*/
FrameRateController::FrameRateController(unsigned int frameRate) : time_per_frame(1000.0 / frameRate), loop_start_time(0),
prev_loop_delta_time(0), frame_counter(0) {
}
FrameRateController::~FrameRateController() {

}

void FrameRateController::start_game_loop() {
	loop_start_time = SDL_GetTicks();
}

void FrameRateController::end_game_loop() {
	frame_counter++;
	limit_frame_rate();
	prev_loop_delta_time = SDL_GetTicks() - loop_start_time;
	float avg_fps = frame_counter / (SDL_GetTicks() / 1000.0f);
	std::string log_msg = "FPS : " + std::to_string(avg_fps);
}

void FrameRateController::limit_frame_rate() {
	Uint32 delayTime = std::max(0.0f, time_per_frame - (SDL_GetTicks() - loop_start_time));
	SDL_Delay(delayTime);
}

Uint32 FrameRateController::GetLoopStartTime() {
	return loop_start_time;
}

Uint32 FrameRateController::GetPrevLoopDeltaTime() {
	return prev_loop_delta_time;
}
