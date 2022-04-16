#include "LogoScreen.h"
#include "GraphicsManager.h"
#include "Util.h"
#include "FrameRateController.h"
#include "ResourceManager.h"
#include "Texture.h"

LogoScreen::LogoScreen(unsigned int _fade_time, unsigned int _screen_time, 
						glm::vec4 _color, std::string tex_name, bool _fade_out, bool _full_screen) :
	Component("LOGOSCREEN"),
	fade_time(_fade_time), screen_time(_screen_time), back_color(_color), 
	background_vao_id(0), completed(false), timer(0), back_vertex_buffer_id(0), 
	back_color_buffer_id(0), back_texture_buffer_id(0), back_vertex_coords(), 
	back_color_coords(), fade_out(_fade_out), image_vao_id(0), 
	vertex_buffer_id(0), color_buffer_id(0), texture_buffer_id(0),
	vertex_coords(), color_coords(), tex_coords() {

	//Full screen quad
	back_vertex_coords = {
		-1,  1, 0,
		-1, -1, 0,
		 1, -1, 0,
		 1,  1, 0
	};

	for (unsigned int i = 0; i < 4; ++i) {
		back_color_coords.push_back(back_color.x);
		back_color_coords.push_back(back_color.y);
		back_color_coords.push_back(back_color.z);
		back_color_coords.push_back(back_color.w);
	}

	tex_coords = {
		0,  0,
		0,  1,
		1,  1,
		1,  0,
	};

	background_vao_id = p_graphics_manager->GenerateQuadVAO(&back_vertex_coords[0], &back_color_coords[0], &tex_coords[0]);

	if (!_full_screen)
	{
		//Logo image
		vertex_coords = {
			-0.70,  0.3, 0,
			-0.70, -0.3, 0,
			 0.70, -0.3, 0,
			 0.70,  0.3, 0
		};
	}
	else
	{
		//Logo image
		vertex_coords = {
			-1,  1, 0,
			-1, -1, 0,
			 1, -1, 0,
			 1,  1, 0
		};
	}

	

	

	for (unsigned int i = 0; i < 4; ++i) {
		color_coords.push_back(back_color.x);
		color_coords.push_back(back_color.y);
		color_coords.push_back(back_color.z);
		color_coords.push_back(back_color.w);
	}

	tex_coords = {
		0,  0,
		0,  1,
		1,  1,
		1,  0,
	};

	image_vao_id = p_graphics_manager->GenerateQuadVAO(&vertex_coords[0], &color_coords[0], &tex_coords[0]);

	transparency_step = 1.0f / fade_time;

	p_resource_manager->add_texture(tex_name);
	p_logo_tex = p_resource_manager->get_texture(tex_name);
}

void LogoScreen::Update() {
	if (timer >= screen_time) {
		completed = true;
		return;
	}
	timer += p_framerate_controller->GetPrevLoopDeltaTime();
	transparency = glm::min(1.0f, (timer / fade_time));
}

bool LogoScreen::isComplete() {
	return completed;
}

void LogoScreen::Draw(ShaderProgram* p_program) {
	p_graphics_manager->SetActiveShader("logo");
	p_graphics_manager->SetDepthTestOff();
	//Set transparency of 1 for background
	p_graphics_manager->SetUniformFloat(1, "transparency");
	p_graphics_manager->SetUniformInt(0, "texture_enabled");
	p_graphics_manager->DrawQuad(background_vao_id, 1, false);

	//Set variable transparency for logo
	if (fade_out)
		p_graphics_manager->SetUniformFloat(transparency, "transparency");
	else
		p_graphics_manager->SetUniformFloat(glm::max(1 - transparency, 0.0f), "transparency");

	p_graphics_manager->SetUniformInt(1, "texture_enabled");
	p_logo_tex->Bind(1, p_graphics_manager->GetActiveShaderId(), "texture_map");
	p_graphics_manager->DrawQuad(image_vao_id, 1, false);
	p_graphics_manager->SetDepthTestOn();
	p_graphics_manager->SetActiveShader("final");
}
