#pragma once

#include "Component.h"
#include "SDL_rect.h"
#include "Matrix3D.h"

class Transform : public Component {
public:
	Transform();
	SDL_Rect GetPosition();
	float GetRotation();
	float GetScaleX();
	float GetScaleY();
	void SetPosition(SDL_Rect new_position);
	void SetRotation(float new_rotation);
	void SetScale(float _scale_x, float _scale_y);
	void Update();
	Matrix3D GetTranslateMatrix();
	Matrix3D GetRotateMatrix();
	Matrix3D GetScaleMatrix();
	Matrix3D GetPreRotateMatrix();
	Matrix3D GetPostRotateMatrix();
	void SetRotMatrices();

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