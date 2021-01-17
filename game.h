#pragma once

#include "Character.h"
#include "gameobject.h"
#include <vector>

enum class GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_DEBUG
};

enum class Direction
{
	X_POS,
	X_NEG,
	Y_POS,
	Y_NEG,
	Z_POS,
	Z_NEG
};

// Type to contain data about collisions between the player and other objects
typedef std::tuple<bool, Direction, glm::vec3> Collision;

class Game
{
public:
	Game(int width, int height);
	void init();
	void processInput();
	void update(double deltaTime);
	void render();
	void setKeyState(int key, bool pressed);
	void setMouseInput(float xOffset, float yOffset);
	void setScrollInput(float xOffset, float yOffset);

private:
	GameState State;
	bool Keys[1024];
	int ScreenWidth;
	int ScreenHeight;

	std::vector<std::unique_ptr<GameObject>> GameObjects;
	std::vector<Shader> Shaders;
	Character PlayerCharacter;
	
	void doCollisions();
	void applyGravity();
	void checkGameOver();
	static Collision checkCollision(const Character& camera, const GameObject& object);
	static Direction getVectorDirection(const glm::vec3& target);
};