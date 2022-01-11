#include "Transform.h"

const float PI = 3.14159f;

Transform::Transform() : Component("TRANSFORM"), position(), rotation(0), scale_x(1), scale_y(1) {}

SDL_Rect Transform::GetPosition() {
	return position;
}

float Transform::GetRotation() {
	return rotation;
}

float Transform::GetScaleX() {
	return scale_x;
}

float Transform::GetScaleY() {
	return scale_y;
}

void Transform::SetPosition(SDL_Rect new_position) {
	position = new_position;
}

void Transform::SetRotation(float new_rotation) {
	rotation = new_rotation;
}

void Transform::SetScale(float _scale_x, float _scale_y) {
	scale_x = _scale_x;
	scale_y = _scale_y;
}

/*
void Transform::Serialize(json json_object) {
}*/

void Transform::SetRotMatrices() {
	pre_rotate_matrix.SetVal(0, 3, -(position.w / 2.0));
	pre_rotate_matrix.SetVal(1, 3, -(position.h / 2.0));

	post_rotate_matrix.SetVal(0, 3, position.w / 2.0);
	post_rotate_matrix.SetVal(1, 3, position.h / 2.0);
}

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

Matrix3D Transform::GetTranslateMatrix() {
	return translate_matrix;
}

Matrix3D Transform::GetRotateMatrix() {
	return rotate_matrix;
}

Matrix3D Transform::GetPreRotateMatrix() {
	return pre_rotate_matrix;
}

Matrix3D Transform::GetPostRotateMatrix() {
	return post_rotate_matrix;
}

Matrix3D Transform::GetScaleMatrix() {
	return scale_matrix;
}
