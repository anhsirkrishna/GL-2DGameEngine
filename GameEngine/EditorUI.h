#pragma once
typedef struct SDL_Window;
typedef void* SDL_GLContext;
class GameObjectManager;


class Editor
{
	GameObjectManager* obj_manager;

public:

	Editor(GameObjectManager* manager_) : obj_manager(manager_) {};

	void Init(SDL_Window* window, SDL_GLContext context) const;
	void NewFrame() const;
	void Render();
	void Cleanup() const;
};

extern Editor* p_editor;

