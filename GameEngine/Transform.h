#pragma once

#include "Component.h"
#include "SDL_rect.h"
#include "Matrix3D.h"

class Transform : public Component {
public:
	//Default ctor for Transform component
	Transform();

	//Returns the SDL_rect that describes the position
	SDL_Rect GetPosition();

	//Returns the Rotation angle in radians
	float GetRotation();

	//Returns the Scale X value
	float GetScaleX();

	//Returns the Scale Y value
	float GetScaleY();

	//Sets a new SDL_rect as the position
	void SetPosition(SDL_Rect const& new_position);

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
	Matrix3D GetTranslateMatrix();

	//Returns the rotation matrix
	Matrix3D GetRotateMatrix();

	//Returns the scale matrix
	Matrix3D GetScaleMatrix();

	//Returns the PreRotateMatrix
	Matrix3D GetPreRotateMatrix();

	//Returns the PostRotateMatrix
	Matrix3D GetPostRotateMatrix();

	//Sets the RotMatrices
	void SetRotMatrices();

	//TO-DO : Implement this when the Serializer is added
	//void Serialize(json json_object);
private:
	SDL_Rect position;
	Matrix3D translate_matrix;
	Matrix3D rotate_matrix;
	Matrix3D scale_matrix;
	Matrix3D pre_rotate_matrix;
	Matrix3D post_rotate_matrix;
	float rotation;
	float scale_x, scale_y;
};