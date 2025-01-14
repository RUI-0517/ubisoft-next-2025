#include "pch.h"
#include "Simplex.h"

void Simplex::expand(const Vector3f& expandPoint)
{
	remove_visible_faces(expandPoint);
	construct_convex(expandPoint);
}

size_t Simplex::getClosestFaceIndex() const
{
	size_t faceIndex = -1;
	float closestDistance = std::numeric_limits<float>::infinity();

	for (size_t i = 0; i < m_faceIndices.size(); i++)
	{
		const float distance = m_distances[i];

		if (distance - 1e-3 <= 0 || distance > closestDistance) continue;

		closestDistance = distance;
		faceIndex = i;
	}

	return faceIndex;
}

Vector3f Simplex::getNormal(const size_t index) const
{
	return m_normals[index];
}

float Simplex::getDistance(const size_t index) const
{
	return m_distances[index];
}

void Simplex::add_face(const size_t v0, const size_t v1, const size_t v2)
{
	Vector3f faceIndices{static_cast<float>(v0), static_cast<float>(v1), static_cast<float>(v2)};

	const Vector3f& a = m_vertices[v0];
	const Vector3f& b = m_vertices[v1];
	const Vector3f& c = m_vertices[v2];

	const Vector3f ab = b - a;
	const Vector3f ac = c - a;

	Vector3f normal = ab.cross(ac).normalize();

	float distance = normal.dot(a);
	if (normal.dot(a) < 0)
	{
		// ensure point outside
		normal *= -1;
		distance *= -1;
		// ensure ccw
		std::swap(faceIndices[1], faceIndices[2]);
	}

	m_faceIndices.push_back(faceIndices);
	m_normals.push_back(normal);
	m_distances.push_back(distance);
}

std::tuple<const Vector3f&, const Vector3f&, const Vector3f&> Simplex::get_vertices_by_face(
	const size_t faceIndex) const
{
	const Vector3f& currentFace = m_faceIndices[faceIndex];

	const auto v0 = static_cast<size_t>(currentFace[0]);
	const auto v1 = static_cast<size_t>(currentFace[1]);
	const auto v2 = static_cast<size_t>(currentFace[2]);

	return {m_vertices[v0], m_vertices[v1], m_vertices[v2]};
}

void Simplex::remove_visible_faces(const Vector3f& point)
{
	std::vector<size_t> removalIndices;

	for (size_t i = 0; i < m_faceIndices.size(); ++i)
	{
		const auto& [v0, v1, v2] = get_vertices_by_face(i);

		const auto& normal = m_normals[i];

		const bool faceCanBeSeenByPoint = normal.dot(point - v0) > 1e-3f;
		if (faceCanBeSeenByPoint) removalIndices.push_back(i);
	}

	for (auto it = removalIndices.rbegin(); it != removalIndices.rend(); ++it)
	{
		const size_t index = *it;
		const Vector3f& removingFace = *(m_faceIndices.begin() + index);

		size_t x = static_cast<size_t>(removingFace.x);
		size_t y = static_cast<size_t>(removingFace.y);
		size_t z = static_cast<size_t>(removingFace.z);

		add_directed_edge({x, y});
		add_directed_edge({y, z});
		add_directed_edge({z, x});

		m_faceIndices.erase(m_faceIndices.begin() + index);
		m_normals.erase(m_normals.begin() + index);
		m_distances.erase(m_distances.begin() + index);
	}
}

void Simplex::add_directed_edge(std::pair<size_t, size_t>&& edge)
{
	const std::pair reversedEdge = {edge.second, edge.first};
	if (m_directedEdges.find(reversedEdge) != m_directedEdges.end())
		m_directedEdges.erase(reversedEdge);
	else m_directedEdges.insert(edge);
}

void Simplex::construct_convex(const Vector3f& point)
{
	m_vertices.push_back(point);
	const size_t index = m_vertices.size() - 1;

	for (const auto& directedEdge : m_directedEdges)
	{
		const size_t c = static_cast<size_t>(std::get<0>(directedEdge));
		const size_t b = static_cast<size_t>(std::get<1>(directedEdge));
		const size_t a = index;

		add_face(c, b, a);
	}

	m_directedEdges.clear();
}
