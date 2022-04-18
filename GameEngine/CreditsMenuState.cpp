#include "CreditsMenuState.h"
#include "LogoScreen.h"
#include "GameObject.h"
#include "StatestackManager.h"
#include "InputManager.h"

#include <SDL.h>

CreditsMenuState::CreditsMenuState() {
	GameObject* p_logo_screen = new GameObject("credits_menu_screen");
	logo_screen.push_back(p_logo_screen);
	LogoScreen* p_logo_screen_comp = new LogoScreen(1000, 30000, glm::vec4(0, 0, 0, 1), "credits", true, true);
	p_logo_screen->AddComponent(p_logo_screen_comp);
	logo_screen_comp.push_back(p_logo_screen_comp);
}

CreditsMenuState::~CreditsMenuState() {
	for (auto& obj : delete_objs)
		delete obj;
}

void CreditsMenuState::Enter() {
	SDL_ShowCursor(SDL_ENABLE);
}


void CreditsMenuState::Update() {
	if (logo_screen.size() == 0) {
		p_statestack_manager->Pop();
		return;
	}
	p_input_manager->Update();

	if (p_input_manager->isMouseTriggered(SDL_BUTTON_LEFT) || p_input_manager->isKeyTriggered(SDL_SCANCODE_ESCAPE) || p_input_manager->isControllerButtonTriggered(SDL_CONTROLLER_BUTTON_START)) {
		delete_objs.push_back(logo_screen.back());
		logo_screen.pop_back();
		logo_screen_comp.pop_back();
		return;
	}

	logo_screen.back()->Update();
	if (logo_screen_comp.back()->isComplete()) {
		delete_objs.push_back(logo_screen.back());
		logo_screen.pop_back();
		logo_screen_comp.pop_back();
	}
}

void CreditsMenuState::Render() {
	if (logo_screen.size() == 0)
		return;

	logo_screen.back()->Draw(nullptr);
}
