#include "stdafx.h"
#include "Camera.h"

#include "App/AppSettings.h"

Camera::Camera(const Vector3f& position, const Vector3f& lookAt): position(position), lookAt(lookAt)
{
	update();
}

const Vector3f& Camera::GetPosition() const
{
	return position;
}

Vector3f& Camera::GetPosition()
{
	return position;
}

Vector3f Camera::getDirection(const float u, const float v) const
{
	return Vector3f{u * m_right + v * m_up + m_focalLength * m_forward}.normalize();
}

void Camera::setFov(const float fov)
{
	this->fov = fov;
	update();
}

void Camera::setPosition(const Vector3f& position)
{
	this->position = position;
	update();
}

void Camera::setLookAt(const Vector3f& position)
{
	lookAt = position;
	update();
}

float Camera::calculate_depth(const float fov)
{
	const float halfFov = fov / 2.0f;
	const float halfFovRadians = halfFov * (PI / 180.0f); // Convert degrees to radians
	return 1.0f / tan(halfFovRadians);
}

void Camera::update()
{
	m_focalLength = calculate_depth(fov);

	// camera coordinate system
	m_forward = (lookAt - position).normalize();
	m_right = Vector3f{0.0f, 1.0f, 0.0f}.cross(m_forward).normalize();
	m_up = m_forward.cross(m_right);
}
