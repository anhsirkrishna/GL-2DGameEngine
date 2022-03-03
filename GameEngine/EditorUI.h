/******************************************************************************/
/*!
/*File: Editor.h
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   1/12/2022
/*  Definition of the Editor Class
*
/* DigiPen Institute of Technology � 2022
/******************************************************************************/
#pragma once
typedef struct SDL_Window;
typedef void* SDL_GLContext;

#include <glm.hpp>
#include <string>
#include <map>

/*
Custom ImGui Editor class
*/
class Editor
{

	std::map<std::string, std::string> obj_def_json_strings;

	int selected = 0;
	int current_index = 0;
	int obj_def_selected = 0;

public:

	

	float last_frame_fps;
	float mouse_x;
	float mouse_y;
	glm::vec4 mouse_worldpos;
	int level_num;

	void Init();
	void NewFrame() const;
	void EditorCameraControls();
	void DebuggerWindow();
	void GameObjectWindow();
	void Render();
	void CreateObject(std::string obj_def);
	void Cleanup() const;
};

extern Editor* p_editor;

