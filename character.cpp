#include "character.h"

Character::Character(const glm::vec3& pos, const glm::vec3& siz, float radius) : GameObject{pos, siz}, Front{glm::vec3{0.0f, 0.0f, -1.0f}}, Forward{glm::vec3{0.0f, 0.0f, -1.0f}}, Up{glm::vec3{0.0f, 1.0f, 0.0f}}, Right{}, WorldUp{Up}, Yaw{-90.0f}, Pitch{0.0f}, MovementSpeed{2.5f}, MouseSensitivity{0.1f}, Fov{45.0f}, Radius{radius}
{
	updateCameraVectors();
}

void Character::processKeyboard(CameraMovement direction)
{
	if (direction == CameraMovement::FORWARD)
		addMovementInput(Front * MovementSpeed);
	if (direction == CameraMovement::BACKWARD)
		addMovementInput(-Front * MovementSpeed);
	if (direction == CameraMovement::RIGHT)
		addMovementInput(Right * MovementSpeed);
	if (direction == CameraMovement::LEFT)
		addMovementInput(-Right * MovementSpeed);
	if (direction == CameraMovement::UP)
		addMovementInput(Up * MovementSpeed);
	if (direction == CameraMovement::DOWN)
		addMovementInput(-Up * MovementSpeed);
	if (direction == CameraMovement::JUMP)
		addMovementInput(WorldUp * MovementSpeed * 3.0f);

	//// Constrain to ground level
	//Position.y = 0.0f;
}

void Character::processMouseMovement(float xOffset, float yOffset)
{
	Yaw += xOffset * MouseSensitivity;
	Pitch += yOffset * MouseSensitivity;

	// Constrain pitch to prevent screen flipping
	constexpr auto pitchMax{89.0f};
	constexpr auto pitchMin{-89.0f};

	if (Pitch > pitchMax)
		Pitch = pitchMax;
	if (Pitch < pitchMin)
		Pitch = pitchMin;

	// Update Front, Right, and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

glm::mat4 Character::getViewMatrix() const
{
	return lookAt(Position, Position + Front, Up);
}

void Character::updateCameraVectors()
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
