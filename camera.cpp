#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Position{position}, WorldUp{up}, Yaw{yaw}, Pitch{pitch}
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Position{glm::vec3{posX, posY, posZ}}, WorldUp{glm::vec3{upX, upY, upZ}}, Yaw{yaw}, Pitch{pitch}
{
	updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	const auto velocity{MovementSpeed * deltaTime};
	if (direction == CameraMovement::FORWARD)
		Position += Front * velocity;
	if (direction == CameraMovement::BACKWARD)
		Position -= Front * velocity;
	if (direction == CameraMovement::LEFT)
		Position -= Right * velocity;
	if (direction == CameraMovement::RIGHT)
		Position += Right * velocity;

	//// Constrain to ground level
	//Position.y = 0.0f;
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	// Constrain pitch to prevent screen flipping
	constexpr auto pitchMax{89.0f};
	constexpr auto pitchMin{-89.0f};
	if (constrainPitch)
	{
		if (Pitch > pitchMax)
			Pitch = pitchMax;
		if (Pitch < pitchMin)
			Pitch = pitchMin;
	}

	// Update Front, Right, and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	Fov -= yOffset;
	
	constexpr auto zoomMin{1.0f};
	constexpr auto zoomMax{45.0f};
	
	if (Fov < zoomMin)
		Fov = zoomMin;
	if (Fov > zoomMax)
		Fov = zoomMax;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	//const auto dirX{cos(glm::radians(Yaw)) * cos(glm::radians(Pitch))};
	const auto dirX{cos(glm::radians(Yaw)) * cos(glm::radians(Pitch))};
	const auto dirY{sin(glm::radians(Pitch))};
	const auto dirZ{sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))};
	const auto front{glm::vec3{dirX, dirY, dirZ}};
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}