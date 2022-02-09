#pragma once

#include <glm.hpp>
#include "nlohmann/json.hpp"

#include "Component.h"

using json = nlohmann::json;

class Transform : public Component {
public:
	//Default ctor for Transform component
	Transform();

	//Returns the SDL_rect that describes the position
	glm::vec4 GetPosition();

	//Returns the Rotation angle in radians
	float GetRotation();

	//Returns the Scale X value
	float GetScaleX();

	//Returns the Scale Y value
	float GetScaleY();

	//Sets a new SDL_rect as the position
	void SetPosition(glm::vec4 const& new_position);

	//Sets a new rotation angle in radians
	void SetRotation(float const& new_rotation);

	//Sets a new scale for X and Y
	void SetScale(float const& _scale_x, float const& _scale_y);

	/*
	* Update called once every game loop for the transform component
	* Sets the matrices up.
	*/
	void Update();

	//Returns the translation matrix
	glm::mat4 GetTranslateMatrix();

	//Returns the rotation matrix
	glm::mat4 GetRotateMatrix();

	//Returns the scale matrix
	glm::mat4 GetScaleMatrix();

	//Returns the PreRotateMatrix
	glm::mat4 GetPreRotateMatrix();

	//Returns the PostRotateMatrix
	glm::mat4 GetPostRotateMatrix();

	//Sets the RotMatrices
	void SetRotMatrices();
	
	//Serialize method. Nothing to do for Transform component.
	void Serialize(json json_object);
private:
	glm::vec4 position;
	glm::mat4 translate_matrix;
	glm::mat4 rotate_matrix;
	glm::mat4 scale_matrix;
	glm::mat4 pre_rotate_matrix;
	glm::mat4 post_rotate_matrix;
	float rotation;
	float scale_x, scale_y;
};