#pragma once
#include <unordered_set>

struct PairHash
{
	size_t operator()(const std::pair<size_t, size_t>& p) const
	{
		return std::hash<size_t>()(p.first) ^ (std::hash<size_t>()(p.second) << 1);
	}
};

class Simplex
{
	std::vector<Vector3f> m_vertices;
	// TODO: using Vector3i
	std::vector<Vector3f> m_faceIndices;
	std::vector<Vector3f> m_normals;
	std::vector<float> m_distances;

	// TODO: using Vector2i
	std::unordered_set<std::pair<size_t, size_t>, PairHash> m_directedEdges;

public:
	explicit Simplex(std::vector<Vector3f>&& vertices)
	{
		m_vertices = std::move(vertices);
		const size_t size = m_vertices.size();

		// initialize data
		m_faceIndices.reserve(size);
		m_normals.reserve(size);
		m_distances.reserve(size);

		for (size_t i = 0; i < size; ++i)
			for (size_t j = i + 1; j < size; ++j)
				for (size_t k = j + 1; k < size; ++k)
					add_face(i, j, k);
	}

	void expand(const Vector3f& expandPoint);

	[[nodiscard]] size_t getClosestFaceIndex() const;

	[[nodiscard]] Vector3f getNormal(size_t index) const;
	[[nodiscard]] float getDistance(size_t index) const;

private:
	// ccw
	void add_face(size_t v0, size_t v1, size_t v2);

	using FaceVertices = std::tuple<const Vector3f&, const Vector3f&, const Vector3f&>;
	[[nodiscard]] FaceVertices get_vertices_by_face(size_t faceIndex) const;
	void remove_visible_faces(const Vector3f& point);
	void add_directed_edge(std::pair<size_t, size_t>&& edge);
	void construct_convex(const Vector3f& point);
};
