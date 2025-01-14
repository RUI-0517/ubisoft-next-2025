#pragma once
#include "Body.h"
#include "CollisionInfo.h"

class Geometry
{
protected:
	std::shared_ptr<Body> m_body;
	std::vector<Vector3f> m_vertices;

public:
	virtual ~Geometry() = default;

	[[nodiscard]] virtual Vector3f getSupportPoint(const Vector3f& direction) const;

	[[nodiscard]] const std::shared_ptr<Body>& getBody() const;
	void attachBody(const std::shared_ptr<Body>& body);

	[[nodiscard]] const std::vector<Vector3f>& getVertices() const
	{
		return m_vertices;
	}

	const std::vector<Vector3f>& getVertices()
	{
		return m_vertices;
	}

	void setVertices(const std::vector<Vector3f>& vertices)
	{
		m_vertices = vertices;
	}

	void setVertices(std::vector<Vector3f>&& vertices)
	{
		m_vertices = std::move(vertices);
	}

	static Vector3f getSupportPoint(const Geometry& lhs, const Geometry& rhs, const Vector3f& direction);
	static std::tuple<bool, std::vector<Vector3f>> checkCollision(const Geometry& lhs, const Geometry& rhs);
	static bool updateCurrentDirection(std::vector<Vector3f>& vertices, Vector3f& currentDirection);

	static CollisionInfo calculateCollisionInfo(std::vector<Vector3f>&& vertices, const Geometry& self,
	                                            const Geometry& other);
};
