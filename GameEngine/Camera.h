/******************************************************************************/
/*!
/*File: Camera.h
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   1/27/2022
/*  Definition of the Camera Class
* 
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// enum for camera controls
enum CameraMovement {
	CAM_FORWARD,
	CAM_BACKWARD,
	CAM_LEFT,
	CAM_RIGHT,
	CAM_UP,
	CAM_DOWN,
	CAM_ZOOMIN,
	CAM_ZOOMOUT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.2f;
const float ZOOM = 45.0f;
const float ZOOM_SPEED = 2.0f;

/*
Basic 3D-Camera class that can work with 2D
*/
class Camera {

public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;

	float yaw;
	float pitch;
	float movement_speed;
	float zoom;
	float zoom_speed;

	Camera(glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up_ = glm::vec3(0.0f, -1.0f, 0.0f), float yaw_ = YAW, float pitch_ = PITCH) : front(glm::vec3(0.0f, 0.0f, 1.0f)), movement_speed(SPEED), zoom(ZOOM), zoom_speed(ZOOM_SPEED)
	{
		position = position_;
		world_up = up_;
		yaw = yaw_;
		pitch = pitch_;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix() { return glm::lookAt(position, position + front, up); }
	void ProcessKeyboardInput(CameraMovement direction, float dt);
	void ProcessZoom(CameraMovement direction);

private:

	void UpdateCameraVectors();

};

//Declared as an extern variable so it can be accessed throughout the project
extern Camera* p_camera;