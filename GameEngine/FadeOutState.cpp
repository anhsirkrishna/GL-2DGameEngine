#include "FadeOutState.h"
#include "FadeOutScreen.h"
#include "GameObject.h"
#include "StatestackManager.h"

FadeOutState::FadeOutState() {
	fade_screen = new GameObject("fade_screen");
	fade_screen_comp = new FadeScreen(1500, glm::vec4(0, 0, 0, 1));
	fade_screen->AddComponent(fade_screen_comp);
}

FadeOutState::~FadeOutState() {
	delete fade_screen;
}

void FadeOutState::Update() {
	fade_screen->Update();
	if (fade_screen_comp->isComplete())
		p_statestack_manager->Pop();
}

void FadeOutState::Render() {
	fade_screen->Draw(nullptr);
}
