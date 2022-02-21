#include "GameObject.h"
#include "Component.h"

//Creates a named game object
GameObject::GameObject(std::string object_name) : name(object_name), index(0) {
	state_manager.SetOwner(this);
}

GameObject::~GameObject() {
	for (unsigned int i = 0; i < component_list.size(); ++i) {
		delete component_list[i];
	}
	component_list.clear();
}

//Returns the name of the Game object
std::string GameObject::GetName() {
	return name;
}

const std::string& GameObject::GetNameRef() const
{
	return name;
}

//Checks if the game object has a particular component. Returns it if present
Component* GameObject::HasComponent(std::string component_name) const {
	for (auto component : component_list) {
		if (component->GetName() == component_name)
			return component;
	}

	return NULL;
}

//Adds a component to the list of components of the game object
void GameObject::AddComponent(Component* component) {
	component_list.push_back(component);
	component->SetOwner(this);
}


//Calls the Update() function for each of its components
void GameObject::Update() {
	for (auto component : component_list) {
		component->Update();
	}
}

/*
* Calls the Link() function for each of its components
* Should be called only after all the components have been added to the game object
*/
void GameObject::LinkComponents() {
	for (auto component : component_list) {
		component->Link();
	}
}

//Calls the Draw() function for each of its components
void GameObject::Draw(ShaderProgram* program) {
	for (auto component : component_list) {
		component->Draw(program);
	}
}
