#pragma once
#include "Body.h"

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
};
