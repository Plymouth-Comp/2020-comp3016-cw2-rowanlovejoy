#include "character.h"

Character::Character(const glm::vec3& pos, const glm::vec3& siz, float radius) : GameObject{pos, siz}, Front{glm::vec3{0.0f, 0.0f, -1.0f}}, Forward{glm::vec3{0.0f, 0.0f, -1.0f}}, Up{glm::vec3{0.0f, 1.0f, 0.0f}}, Right{}, WorldUp{Up}, Yaw{-90.0f}, Pitch{0.0f}, MovementSpeed{0.035f}, MouseSensitivity{0.1f}, Fov{85.0f}, Radius{radius}, Grounded{false}
{
	updateCameraVectors();
}

void Character::processKeyboard(PlayerMovement direction)
{
	constexpr auto startingJumpVel{0.25f};
	static auto jumping{false};
	static auto jumpPressed{false};
	static auto jumpCount{0};
	static auto jumpVel{startingJumpVel};

	if (direction == PlayerMovement::FORWARD)
		Position += Forward * MovementSpeed;
	if (direction == PlayerMovement::BACKWARD)
		Position += -Forward * MovementSpeed;
	if (direction == PlayerMovement::RIGHT)
		Position += Right * MovementSpeed;
	if (direction == PlayerMovement::LEFT)
		Position += -Right * MovementSpeed;
	if (direction == PlayerMovement::UP)
		Position += Up * MovementSpeed;
	if (direction == PlayerMovement::DOWN)
		Position += -Up * MovementSpeed;

	if (direction == PlayerMovement::JUMP_PRESSED && Grounded && !jumpPressed)
	{
		jumping = true;

		Grounded = false;

		jumpPressed = true;
	}

	// If jump button is released at this point, it won't take affect until the next iteration
	if (direction == PlayerMovement::JUMP_RELEASED)
	{
		jumpPressed = false;
	}

	if (jumping)
	{
		Position += WorldUp * jumpVel;
		++jumpCount;
		jumpVel *= 0.95f;

		if (jumpCount >= 180)
		{
			jumping = false;
			jumpCount = 0;
			jumpVel = startingJumpVel;
		}
	}

	// If land early, reset jumping
	if (Grounded)
	{
		jumping = false;
		jumpCount = 0;
		jumpVel = startingJumpVel;
	}
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
	// Calculate new Front vector for camera
	const auto dirX{cos(glm::radians(Yaw)) * cos(glm::radians(Pitch))};
	const auto dirY{sin(glm::radians(Pitch))};
	const auto dirZ{sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))};
	const auto front{glm::vec3{dirX, dirY, dirZ}};
	Front = glm::normalize(front);

	// Calculate new Forward vector for character movement -- Y is zero to prevent flying
	const auto forward{glm::vec3{dirX, 0.0f, dirZ}};
	Forward = glm::normalize(forward);
	
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
