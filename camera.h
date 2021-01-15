#pragma once

#include "gameobject.h"

class Camera
{
public:
	Camera(const GameObject& base);
	
	float getFov() const
	{
		return Fov;
	}

	// Get view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() const;

	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

	void setPitch(float newPitch)
	{
		Pitch = newPitch;
	}

	void setYaw(float newYaw)
	{
		Pitch = newYaw;
	}
	
	float getPitch() const
	{
		return Pitch;
	}

	float getYaw() const
	{
		return Yaw;
	}
private:
	const GameObject& Base;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	float Fov;
};
