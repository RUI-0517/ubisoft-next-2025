#pragma once

struct Camera
{
private:
	Vector3f m_forward;
	Vector3f m_up;
	Vector3f m_right;

	float m_focalLength;

public:
#pragma region API
	Vector3f position;
	Vector3f lookAt;
	float fov = 90.0f;

	Camera(const Vector3f& position, const Vector3f& lookAt);

	[[nodiscard]] Vector3f& GetPosition();
	[[nodiscard]] const Vector3f& GetPosition() const;
	[[nodiscard]] Vector3f getDirection(float u, float v) const;

	void setFov(float fov);
	void setPosition(const Vector3f& position);
	void setLookAt(const Vector3f& position);

	void update();
#pragma endregion

private:
#pragma region Internal
	static float calculate_depth(float fov);
#pragma endregion
};
