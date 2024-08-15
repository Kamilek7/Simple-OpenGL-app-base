#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaders.h"

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	int width;
	int height;

	bool firstClick = true;

	float speed = 0.5f;
	float sensitivity = 100.0f;

	Camera() {};
	Camera(int width, int height, glm::vec3 position);

	void update(float FOVdeg, float nearPlane, float farPlane);
	void matrix(Shader shader, const char* uniform);
	void inputs(GLFWwindow* window, float dt);
};
	

#endif