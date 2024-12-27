#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f, float aspect = 1600.0f / 1000.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	float fov;
	float aspect;
	float nearPlane;
	float farPlane;
};
#endif