#include "pch.h"
#include "Geometry.h"

Vector3f Geometry::GetSupportPoint(const Vector3f& direction)
{
	const auto& transform = m_body->transform;
	const auto& vertices = m_vertices;

	float farthestExtent = -INFINITY;
	Vector3f supportingPoint;

	for (const Vector3f& vertex : vertices)
	{
		Vector3f worldVertex = transform.TransformPoint(vertex);
		const float supportValue = worldVertex.dot(direction);

		if (supportValue < farthestExtent) continue;
		farthestExtent = supportValue;
		supportingPoint = worldVertex;
	}

	return supportingPoint;
}

const std::shared_ptr<Body>& Geometry::getBody() const
{
	return m_body;
}

void Geometry::attachBody(const std::shared_ptr<Body>& body)
{
	if (!body) throw std::invalid_argument("Body cannot be null.");
	m_body = body;
}
