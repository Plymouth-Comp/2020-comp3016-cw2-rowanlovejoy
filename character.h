#pragma once

#include <iostream>

#include "gameobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class PlayerMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	JUMP_PRESSED,
	JUMP_RELEASED
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Character : public GameObject
{
public:
	// Vector constructor
	Character(const glm::vec3& pos, const glm::vec3& siz, float radius);

	// Process input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(PlayerMovement direction);

	// Process input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xOffset, float yOffset);

	// Get view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() const;

	float getFov() const
	{
		return Fov;
	}

	glm::vec3 getFront() const
	{
		return Front;
	}

	float getRadius() const
	{
		return Radius;
	}

	float getMovementSpeed() const
	{
		return MovementSpeed;
	}

	void setMovementSpeed(float newSpeed)
	{
		if (newSpeed < 0.0f)
			newSpeed = 0.0f;

		MovementSpeed = newSpeed;
	}

	bool getGrounded() const
	{
		return Grounded;
	}
	
	void setGrounded(bool grounded)
	{
		Grounded = grounded;
	}

private:
	// camera Attributes
	glm::vec3 Front;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Fov;
	float Radius;

	bool Grounded;
	
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};
