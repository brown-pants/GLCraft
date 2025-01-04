#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float fov, float aspect, float nearPlane, float farPlane)
	: position(position), front(front), up(up), fov(fov), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane) 
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}
