#include "FadeOutScreen.h"
#include "GraphicsManager.h"
#include "Util.h"
#include "FrameRateController.h"

FadeScreen::FadeScreen(unsigned int _fade_time, glm::vec4 _color, bool _fade_out) :
	Component("FADEOUT"),
	fade_time(_fade_time), fade_color(_color), quad_vao_id(0), completed(false),
	timer(0), vertex_buffer_id(0), color_buffer_id(0), texture_buffer_id(0),
	vertex_coords(), color_coords(), tex_coords(), fade_out(_fade_out) {

	//Full screen quad
	vertex_coords = {
		-1,  1, 0,
		-1, -1, 0,
		 1, -1, 0,
		 1,  1, 0
	};

	for (unsigned int i = 0; i < 4; ++i) {
		color_coords.push_back(fade_color.x);
		color_coords.push_back(fade_color.y);
		color_coords.push_back(fade_color.z);
		color_coords.push_back(fade_color.w);
	}

	tex_coords = {
		0,  0,
		0,  1,
		1,  1,
		1,  0,
	};

	quad_vao_id = p_graphics_manager->GenerateQuadVAO(&vertex_coords[0], &color_coords[0], &tex_coords[0]);

	transparency_step = 1.0f / fade_time;
}

void FadeScreen::Update() {
	if (timer >= fade_time) {
		completed = true;
		return;
	}
	timer += p_framerate_controller->GetPrevLoopDeltaTime();
	transparency = (timer / fade_time);
}

bool FadeScreen::isComplete() {
	return completed;
}

void FadeScreen::Draw(ShaderProgram* p_program) {
	p_graphics_manager->SetActiveShader("fade");
	p_graphics_manager->SetDepthTestOff();
	if (fade_out)
		p_graphics_manager->SetUniformFloat(transparency, "transparency");
	else
		p_graphics_manager->SetUniformFloat(glm::max(1-transparency, 0.0f), "transparency");
	p_graphics_manager->DrawQuad(quad_vao_id, 1, false);
	p_graphics_manager->SetDepthTestOn();
	p_graphics_manager->SetActiveShader("final");
}
