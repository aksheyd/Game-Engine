#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "Transform.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
//enum Camera_Movement {
//    FORWARD,
//    BACKWARD,
//    LEFT,
//    RIGHT
//};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public Component
{
public:
	// most recently created camera
	static Camera* main;

	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// euler Angles
	float Yaw;
	float Pitch;

	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Component("Camera"), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

		main = this;
	}

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Component("Camera"), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

		main = this;
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		/*
		custom implementation of lookAt() function

		//glm::mat4 rotation = glm::mat4(1.0f);
		//rotation[0][0] = Right.x;
		//rotation[1][0] = Right.y;
		//rotation[2][0] = Right.z;
		//rotation[0][1] = Up.x;
		//rotation[1][1] = Up.y;
		//rotation[2][1] = Up.z;
		//rotation[0][2] = -Front.x;
		//rotation[1][2] = -Front.y;
		//rotation[2][2] = -Front.z;

		//glm::mat4 translation = glm::mat4(1.0f);
		//translation[3][0] = -Position.x;
		//translation[3][1] = -Position.y;
		//translation[3][2] = -Position.z;
		*/

		//return rotation * translation;

		return glm::lookAt(Position, Position + Front, Up);
	}

	//void Start() {
	//	updateCameraVectors();
	//}

	void Update() {
		Position = transform->position;
	}

	glm::vec3 ChangePosition(const glm::vec3& new_pos) {
		Position = new_pos;
		updateCameraVectors();
	}

	//// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	//void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	//{
	//    float velocity = MovementSpeed * deltaTime;
	//    if (direction == FORWARD)
	//        Position += Front * velocity;
	//    if (direction == BACKWARD)
	//        Position -= Front * velocity;
	//    if (direction == LEFT)
	//        Position -= Right * velocity;
	//    if (direction == RIGHT)
	//        Position += Right * velocity;
	//    /* - Position.y = 0.0f; -> makes this an FPS camera - */
	//}

	//// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	//void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	//{
	//    xoffset *= MouseSensitivity;
	//    yoffset *= MouseSensitivity;

	//    Yaw += xoffset;
	//    Pitch += yoffset;

	//    // make sure that when pitch is out of bounds, screen doesn't get flipped
	//    if (constrainPitch)
	//    {
	//        if (Pitch > 89.0f)
	//            Pitch = 89.0f;
	//        if (Pitch < -89.0f)
	//            Pitch = -89.0f;
	//    }

	//    // update Front, Right and Up Vectors using the updated Euler angles
	//    updateCameraVectors();
	//}

	//// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	//void ProcessMouseScroll(float yoffset)
	//{
	//    Zoom -= (float)yoffset;
	//    if (Zoom < 1.0f)
	//        Zoom = 1.0f;
	//    if (Zoom > 45.0f)
	//        Zoom = 45.0f;
	//}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif