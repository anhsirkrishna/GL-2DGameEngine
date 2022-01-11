#include "Component.h"

//Constructor that creates a named component
Component::Component(std::string _name) : name(_name), p_owner_object(NULL) {}

//Returns the name of the component
std::string Component::GetName() {
	return name;
}

//Sets a GameObject as the owner of the component instance
void Component::SetOwner(GameObject* owner_object) {
	p_owner_object = owner_object;
}

//Returns a pointer to the owner GameObject
GameObject* Component::GetOwner() {
	return p_owner_object;
}

//Base Update method. Does nothing.
void Component::Update() {

}

//Base Draw method. Does nothing.
void Component::Draw(ShaderProgram* p_program) {

}

//Base Link method. Does nothing.
//Individual components will have logic that links itself with other dependant components
void Component::Link() {

}