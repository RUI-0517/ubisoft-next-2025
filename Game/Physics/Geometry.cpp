#include "pch.h"
#include "Geometry.h"

#include <string>

Vector3f Geometry::getSupportPoint(const Vector3f& direction) const
{
	if (m_body == nullptr)
	{
		throw std::runtime_error(
			std::string(__FUNCTION__) +
			": body is not assigned! Please call attachBody to assign a Body to the Geometry object."
		);
	}

	const auto& transform = m_body->transform;
	const auto& vertices = m_vertices;

	float farthestExtent = -INFINITY;
	Vector3f supportingPoint;

	for (const Vector3f& vertex : vertices)
	{
		Vector4f worldVertex = transform.transformPoint(vertex);
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

Vector3f Geometry::getSupportPoint(const Geometry& lhs, const Geometry& rhs, const Vector3f& direction)
{
	return lhs.getSupportPoint(direction) - rhs.getSupportPoint(-direction);
}

// GJK
std::tuple<bool, std::vector<Vector3f>> Geometry::checkCollision(const Geometry& lhs, const Geometry& rhs)
{
	std::vector<Vector3f> vertices;

	const Transform& lhsTransform = lhs.getBody()->transform;
	const Transform& rhsTransform = rhs.getBody()->transform;

	Vector3f currentDirection = rhsTransform.position - lhsTransform.position;

	// First Vertex
	vertices.push_back(getSupportPoint(lhs, rhs, currentDirection));

	// Second Vertex
	{
		updateCurrentDirection(vertices, currentDirection);
		vertices.push_back(getSupportPoint(lhs, rhs, currentDirection));
	}

	// Third Vertex
	{
		updateCurrentDirection(vertices, currentDirection);
		vertices.push_back(getSupportPoint(lhs, rhs, currentDirection));
	}

	bool containsOrigin;
	int currentAttempt = 0;
	constexpr int maxAttempts = 24;
	do
	{
		containsOrigin = updateCurrentDirection(vertices, currentDirection);

		if (containsOrigin) break;

		// Add Support Point
		Vector3f newVertex = getSupportPoint(lhs, rhs, currentDirection);
		const bool noIntersection = currentDirection.dot(newVertex) <= 1e-3f;
		if (noIntersection) break;

		vertices.push_back(newVertex);
		++currentAttempt;
	}
	while (currentAttempt < maxAttempts);

	return std::make_tuple(containsOrigin, containsOrigin ? std::vector<Vector3f>{} : std::move(vertices));
}

/// <summary>
/// 
/// </summary>
/// <param name="vertices"></param>
/// <param name="currentDirection">this direction always be updated</param>
/// <returns>if founded a collision</returns>
bool Geometry::updateCurrentDirection(std::vector<Vector3f>& vertices, Vector3f& currentDirection)
{
	switch (vertices.size())
	{
	case 1:
		{
			currentDirection = -currentDirection;
			return false;
		}

	case 2:
		{
			const Vector3f& a = vertices[0];
			const Vector3f& b = vertices[1];

			const Vector3f ab = b - a;
			const Vector3f a0 = -a;

			currentDirection = Vector3f::tripleProduct(ab, a0, ab).normalize();
			return false;
		}

	case 3:
		{
			const Vector3f& a = vertices[0];
			const Vector3f& b = vertices[1];
			const Vector3f& c = vertices[2];

			const Vector3f ab = b - a;
			const Vector3f ac = c - a;
			const Vector3f a0 = -a;

			currentDirection = ac.cross(ab).normalize();
			if (currentDirection.dot(a0) < 0) currentDirection *= -1;
			return false;
		}

	case 4:
		{
			const Vector3f& a = vertices[0];
			const Vector3f& b = vertices[1];
			const Vector3f& c = vertices[2];
			const Vector3f& d = vertices[3];

			const Vector3f ad = d - a;
			const Vector3f bd = d - b;
			const Vector3f cd = d - c;
			const Vector3f d0 = -d;

			const Vector3f abdNormal = ad.cross(bd);
			const Vector3f bcdNormal = bd.cross(cd);
			const Vector3f cadNormal = cd.cross(ad);

			if (abdNormal.dot(d0) > 0)
			{
				vertices.erase(
					std::remove(vertices.begin(), vertices.end(), c),
					vertices.end()
				);

				currentDirection = abdNormal.normalize();
				return false;
			}

			if (bcdNormal.dot(d0) > 0)
			{
				vertices.erase(
					std::remove(vertices.begin(), vertices.end(), a),
					vertices.end()
				);

				currentDirection = bcdNormal.normalize();
				return false;
			}

			if (cadNormal.dot(d0) > 0)
			{
				vertices.erase(
					std::remove(vertices.begin(), vertices.end(), b),
					vertices.end()
				);

				currentDirection = cadNormal.normalize();
				return false;
			}

			return true;
		}

	default: throw std::invalid_argument("Invalid case number: " + std::to_string(vertices.size()));
	}
}
