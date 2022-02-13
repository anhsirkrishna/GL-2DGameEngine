#pragma once
#include <string>
#include <nlohmann/json.hpp>

//Forward declaration
class GameObject;
class ShaderProgram;

using json = nlohmann::json;

/*
* Base component class that serves as the generic component
* Each individual component will need to implement the virtual methods
* Any behavior will have to be contained within the individual components.
*/
class Component {
public:
	//Constructor that creates a named component
	Component(std::string _name);

	//Sets a GameObject as the owner of the component instance
	void SetOwner(GameObject* p_owner_object);

	//Sets a GameObject as the owner of the component instance
	GameObject* GetOwner();

	//Base Update method. Does nothing.
	virtual void Update();

	//Base Link method. Does nothing.
	//Individual components will have logic that links itself with other dependant components
	virtual void Link();

	//Base Draw method. Does nothing.
	virtual void Draw(ShaderProgram* p_program);

	virtual void ChangeState(json json_object);

	//Returns the name of the component
	std::string GetName();
private:
	std::string name;
	GameObject* p_owner_object;
};
