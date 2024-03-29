#include "Transform.h"
#include "Util.h"
#include "GameObject.h"
#include "GLQuad.h"

#include <SDL.h>

const float PI = 3.14159f;

//Default ctor for Transform component
Transform::Transform() : Component("TRANSFORM"), position(0,0,0,0), rotation(0),
						 scale_x(1), scale_y(1), translate_matrix(1.0),
						 rotate_matrix(1.0), scale_matrix(1.0),
						 pre_rotate_matrix(1.0), post_rotate_matrix(1.0),
						 p_owner_glquad(nullptr) {}

//Returns the SDL_rect that describes the position
glm::vec4 Transform::GetPosition() {
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
void Transform::SetPosition(glm::vec4 const& new_position) {
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
	SetRotMatrices();
}

//Nothing to do for Transform component
void Transform::Serialize(json json_object) {
}

//Sets the RotMatrices
void Transform::SetRotMatrices() {
	if (p_owner_glquad != nullptr) {
		//Translate to origin before rotation
		float quad_width = p_owner_glquad->GetDimensions().x * glm::abs(scale_x);
		float quad_height = p_owner_glquad->GetDimensions().y * glm::abs(scale_y);
		pre_rotate_matrix[3].x = -(quad_width / 2.0);
		pre_rotate_matrix[3].y = -(quad_height / 2.0);

		//Translate to position after rotation
		post_rotate_matrix[3].x = (quad_width / 2.0);
		post_rotate_matrix[3].y = (quad_height / 2.0);
	}
}

/*
* Update called once every game loop for the transform component
* Sets the matrices up.
*/
void Transform::Update() {
	translate_matrix[3] = glm::vec4(glm::vec3(position.x, position.y, position.z), 1.0f);

	scale_matrix[0].x = scale_x;
	scale_matrix[1].y = scale_y;

	rotate_matrix[0].x = cosf(rotation * PI / 180);
	rotate_matrix[1].x = sinf((rotation * PI) / 180);
	rotate_matrix[0].y = -sinf((rotation * PI) / 180);
	rotate_matrix[1].y = cos((rotation * PI) / 180);
}

//Returns the translation matrix
glm::mat4 Transform::GetTranslateMatrix() {
	return translate_matrix;
}

//Returns the rotation matrix
glm::mat4 Transform::GetRotateMatrix() {
	return rotate_matrix;
}

//Returns the PreRotateMatrix
glm::mat4 Transform::GetPreRotateMatrix() {
	return pre_rotate_matrix;
}

//Returns the PostRotateMatrix
glm::mat4 Transform::GetPostRotateMatrix() {
	return post_rotate_matrix;
}

//Returns the scale matrix
glm::mat4 Transform::GetScaleMatrix() {
	return scale_matrix;
}

//Links transform component with GLQuad component
void Transform::Link() {
	p_owner_glquad = static_cast<GLQuad*>(GetOwner()->HasComponent("GLQUAD"));
	SetRotMatrices();
}

/*Flips the transform so it appears to be facing the opposite direction
* in the x axist
* Returns : void
*/
void Transform::FlipTransform() {
	SetScale(GetScaleX() * -1, GetScaleY());
}

/*Sets a specific coordinate
* coord_ specifies which coordnate
* x - 0, y - 1, z - 2, w - 3
* Returns : void
*/
void Transform::SetPosCoord(float val_, int coord_) {
	position[coord_] = val_;
}

/*Gets a specific coordinate
* coord_ specifies which coordnate
* x - 0, y - 1, z - 2, w - 3
* Returns : float - the coordinate
*/
float Transform::GetPosCoord(int coord_) {
	return position[coord_];
}