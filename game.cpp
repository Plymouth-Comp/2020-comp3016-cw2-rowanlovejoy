#include "game.h"
#include "visibleobject.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <tuple>
#include <iostream>

Game::Game(int width, int height) : State{GameState::GAME_ACTIVE}, Keys{}, ScreenWidth{width}, ScreenHeight{height}, GameObjects{}, Shaders{}, PlayerCharacter{glm::vec3{0.0f, 1.5f, 3.0f}, glm::vec3{3.0f}, 1.0f} { }

void Game::init()
{
	// Initialise shaders
	Shaders.emplace_back(Shader{"shaders/shader.vert", "shaders/shader.frag"});

	// Initialise level objects
	const Model platformModel{"media/platform/platform.obj"};
	GameObjects.emplace_back(new VisibleObject{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{2.0f, 1.0f, 2.0f}, platformModel});
	GameObjects.emplace_back(new VisibleObject{glm::vec3{0.0f, 0.0f, -3.0f}, glm::vec3{2.0f, 1.0f, 2.0f}, platformModel});

	// Projection matrix doesn't change so can be initialised here
	const auto projection{glm::perspective(glm::radians(PlayerCharacter.getFov()), static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight), 0.1f, 100.0f)};
	Shaders[0].use();
	Shaders[0].setUniform("projection", projection);
}

void Game::processInput()
{
	// Camera movement
	if (Keys[GLFW_KEY_W])
		PlayerCharacter.processKeyboard(CameraMovement::FORWARD);
	if (Keys[GLFW_KEY_S])
		PlayerCharacter.processKeyboard(CameraMovement::BACKWARD);
	if (Keys[GLFW_KEY_A])
		PlayerCharacter.processKeyboard(CameraMovement::LEFT);
	if (Keys[GLFW_KEY_D])
		PlayerCharacter.processKeyboard(CameraMovement::RIGHT);
	if (Keys[GLFW_KEY_Q])
		PlayerCharacter.processKeyboard(CameraMovement::UP);
	if (Keys[GLFW_KEY_E])
		PlayerCharacter.processKeyboard(CameraMovement::DOWN);
	if (Keys[GLFW_KEY_SPACE])
		PlayerCharacter.processKeyboard(CameraMovement::JUMP);
}

void Game::update(float deltaTime)
{
	PlayerCharacter.move(deltaTime);
	//applyResistance(deltaTime);
	
	// After do object movement, check apply gravity and do collisions
	applyGravity(deltaTime);
	doCollisions();
}

void Game::render()
{
	// Don't render anything without shaders
	if (Shaders.empty())
		return;

	const auto view{PlayerCharacter.getViewMatrix()};
	Shaders[0].use();
	Shaders[0].setUniform("view", view);

	for (const auto& obj : GameObjects)
		obj->draw(Shaders[0]);
}

void Game::doCollisions()
{
	for (const auto& obj : GameObjects)
	{
		const auto collision{checkCollision(PlayerCharacter, *obj)};

		// If collision occurred...
		if (std::get<0>(collision))
		{
			std::cout << "Collision detected\n";

			const auto dir{std::get<1>(collision)};
			const auto diffVector{std::get<2>(collision)};

			if (dir == Direction::X_POS || dir == Direction::X_NEG)
			{
				const auto penetration{PlayerCharacter.getRadius() - std::abs(diffVector.x)};

				if (dir == Direction::X_POS)
				{
					const auto newX{PlayerCharacter.getPosition().x - penetration};
					PlayerCharacter.setPosition(glm::vec3{newX, PlayerCharacter.getPosition().y, PlayerCharacter.getPosition().z});
				}

				if (dir == Direction::X_NEG)
				{
					const auto newX{PlayerCharacter.getPosition().x + penetration};
					PlayerCharacter.setPosition(glm::vec3{newX, PlayerCharacter.getPosition().y, PlayerCharacter.getPosition().z});
				}
			}
			else if (dir == Direction::Y_POS || dir == Direction::Y_NEG)
			{
				const auto penetration{PlayerCharacter.getRadius() - std::abs(diffVector.y)};

				if (dir == Direction::Y_POS)
				{
					const auto newY{PlayerCharacter.getPosition().y - penetration};
					PlayerCharacter.setPosition(glm::vec3{PlayerCharacter.getPosition().x, newY, PlayerCharacter.getPosition().z});
				}

				if (dir == Direction::Y_NEG)
				{
					const auto newY{PlayerCharacter.getPosition().y + penetration};
					PlayerCharacter.setPosition(glm::vec3{PlayerCharacter.getPosition().x, newY, PlayerCharacter.getPosition().z});
				}
			}
			else
			{
				const auto penetration{PlayerCharacter.getRadius() - std::abs(diffVector.z)};

				if (dir == Direction::Z_POS)
				{
					const auto newZ{PlayerCharacter.getPosition().z - penetration};
					PlayerCharacter.setPosition(glm::vec3{PlayerCharacter.getPosition().x, PlayerCharacter.getPosition().y, newZ});
				}

				if (dir == Direction::Z_NEG)
				{
					const auto newZ{PlayerCharacter.getPosition().z + penetration};
					PlayerCharacter.setPosition(glm::vec3{PlayerCharacter.getPosition().x, PlayerCharacter.getPosition().y, newZ});
				}
			}
		}
	}
}

void Game::applyGravity(float deltaTime)
{
	const auto gravity{1.25f * deltaTime};

	const auto newPos{PlayerCharacter.getPosition() - glm::vec3{0.0f, gravity, 0.0f}};

	PlayerCharacter.setPosition(newPos);
}

void Game::applyResistance(float deltaTime)
{
	const auto resistance{1.5f * deltaTime};
	
	PlayerCharacter.addMovementInput(glm::vec3{resistance});
}

Direction Game::getVectorDirection(const glm::vec3& target)
{
	constexpr glm::vec3 directions[]
	{
		glm::vec3{1.0f, 0.0f, 0.0f}, // Positive X
		glm::vec3{-1.0f, 0.0f, 0.0f}, // Negative X
		glm::vec3{0.0f, 1.0f, 0.0f}, // Positive Y
		glm::vec3{0.0f, -1.0f, 0.0f}, // Negative Y
		glm::vec3{0.0f, 0.0f, 1.0f}, // Positive Z
		glm::vec3{0.0f, 0.0f, -1.0f} // Negative Z
	};

	auto max{0.0f};

	auto bestMatch{-1};

	for (auto i{0}; i < 6; ++i)
	{
		const auto dotProduct{glm::dot(glm::normalize(target), directions[i])};

		if (dotProduct > max)
		{
			max = dotProduct;

			bestMatch = i;
		}
	}

	return static_cast<Direction>(bestMatch);
}

void Game::setKeyState(int key, bool pressed)
{
	Keys[key] = pressed;
}

void Game::setMouseInput(float xOffset, float yOffset)
{
	PlayerCharacter.processMouseMovement(xOffset, yOffset);
}

void Game::setScrollInput(float xOffset, float yOffset)
{
	const auto newSpeed{PlayerCharacter.getMovementSpeed() + yOffset};

	PlayerCharacter.setMovementSpeed(newSpeed);
}

Collision Game::checkCollision(const Character& camera, const GameObject& object)
{
	const auto cameraPos{camera.getPosition()};
	const auto cameraRad{camera.getRadius()};

	// Find camera centre by adding radius to position
	const auto cameraCentre{cameraPos + cameraRad};

	// Find ABB centre and half-extents
	const auto objectSize{object.getSize()};
	const auto objectPos{object.getPosition()};
	const auto aabbHalfExtents
	{
		glm::vec3
		{
			objectSize.x / 2.0f,
			objectSize.y / 2.0f,
			objectSize.z / 2.0f
	}
	};
	const auto aabbCentre
	{
		glm::vec3
		{
			objectPos.x + aabbHalfExtents.x,
			objectPos.y + aabbHalfExtents.y,
			objectPos.z + aabbHalfExtents.z
	}
	};

	// Find difference between both centres
	const auto difference{cameraCentre - aabbCentre};
	const auto clampedDiff{glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents)};

	// Find position closest to camera (the circle)
	const auto closestPoint{aabbCentre + clampedDiff};

	// Get vector between circle's centre and closest AABB point, and check if length is less than or equal to circle's radius
	const auto length{closestPoint - cameraCentre};

	// If no collision...
	if (glm::length(length) <= cameraRad)
		return std::make_tuple(true, getVectorDirection(length), length);

	return std::make_tuple(false, Direction::X_POS, glm::vec3{0.0f});
}
