#include "Transform.h"

const float PI = 3.14159f;

//Default ctor for Transform component
Transform::Transform() : Component("TRANSFORM"), position(), rotation(0), scale_x(1), scale_y(1) {}

//Returns the SDL_rect that describes the position
SDL_Rect Transform::GetPosition() {
	return position;
}

//Returns the Rotation angle in radians
float Transform::GetRotation() {
	return rotation;
}

//Returns the Scale X value
float Transform::GetScaleX() {
	return scale_x;
}

//Returns the Scale Y value
float Transform::GetScaleY() {
	return scale_y;
}

//Sets a new SDL_rect as the position
void Transform::SetPosition(SDL_Rect const& new_position) {
	position = new_position;
}

//Sets a new rotation angle in radians
void Transform::SetRotation(float const& new_rotation) {
	rotation = new_rotation;
}

//Sets a new scale for X and Y
void Transform::SetScale(float const&  _scale_x, float const&  _scale_y) {
	scale_x = _scale_x;
	scale_y = _scale_y;
}

/*
void Transform::Serialize(json json_object) {
}*/

//Sets the RotMatrices
void Transform::SetRotMatrices() {
	pre_rotate_matrix.SetVal(0, 3, -(position.w / 2.0));
	pre_rotate_matrix.SetVal(1, 3, -(position.h / 2.0));

	post_rotate_matrix.SetVal(0, 3, position.w / 2.0);
	post_rotate_matrix.SetVal(1, 3, position.h / 2.0);
}

/*
* Update called once every game loop for the transform component
* Sets the matrices up.
*/
void Transform::Update() {
	translate_matrix.SetVal(0, 3, position.x);
	translate_matrix.SetVal(1, 3, position.y);

	scale_matrix.SetVal(0, 0, scale_x);
	scale_matrix.SetVal(1, 1, scale_y);

	rotate_matrix.SetVal(0, 0, cosf(rotation * PI / 180));
	rotate_matrix.SetVal(0, 1, sinf((rotation * PI) / 180));
	rotate_matrix.SetVal(1, 0, -sinf((rotation * PI) / 180));
	rotate_matrix.SetVal(1, 1, cos((rotation * PI) / 180));
}

//Returns the translation matrix
Matrix3D Transform::GetTranslateMatrix() {
	return translate_matrix;
}

//Returns the rotation matrix
Matrix3D Transform::GetRotateMatrix() {
	return rotate_matrix;
}

//Returns the PreRotateMatrix
Matrix3D Transform::GetPreRotateMatrix() {
	return pre_rotate_matrix;
}

//Returns the PostRotateMatrix
Matrix3D Transform::GetPostRotateMatrix() {
	return post_rotate_matrix;
}

//Returns the scale matrix
Matrix3D Transform::GetScaleMatrix() {
	return scale_matrix;
}
