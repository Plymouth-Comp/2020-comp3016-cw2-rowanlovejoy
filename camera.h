#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
constexpr auto YAW{-90.0f};
constexpr auto PITCH{0.0f};
constexpr auto SPEED{2.5f};
constexpr auto SENSITIVITY{0.1f};
constexpr auto FOV{45.0f};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    // camera Attributes
    glm::vec3 Position{glm::vec3{0.0f, 0.0f, 0.0f}};
    glm::vec3 Front{glm::vec3{0.0f, 0.0f, -1.0f}};
    glm::vec3 Forward{glm::vec3{0.0f, 0.0f, -1.0f}};
    glm::vec3 Up{glm::vec3{0.0f, 1.0f, 0.0f}};
    glm::vec3 Right{};
    glm::vec3 WorldUp{};
    // euler Angles
    float Yaw{};
    float Pitch{};
    // camera options
    float MovementSpeed{SPEED};
    float MouseSensitivity{SENSITIVITY};
    float Fov{FOV};

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

public:
    // Vector constructor
    Camera(glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f}, float yaw = YAW, float pitch = PITCH);
	
    // Scalar constructor
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Process input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    // Process input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	
    // Process input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset);

    // Get view matrix calculated using Euler Angles and the LookAt Matrix
    inline glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    inline float GetFov() const
	{
		return Fov;
	}
};