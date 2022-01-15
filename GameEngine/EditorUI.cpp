#include "EditorUI.h"
#include <string>

void ImGuiRender()
{
	ImGui::Begin("Demo Window");
	ImGui::Button("Hello!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiCleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
