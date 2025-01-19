#include "pch.h"
#include "PlaneGeometry.h"

PlaneGeometry::PlaneGeometry(): m_size(1.0f)
{
	setVertices(std::vector{
		Vector3f{-0.5f * m_size, 0.0f, -0.5f * m_size},
		Vector3f{0.5f * m_size, 0.0f, -0.5f * m_size},
		Vector3f{-0.5f * m_size, 0.0f, 0.5f * m_size},
		Vector3f{0.5f * m_size, 0.0f, 0.5f * m_size}
	});
}

PlaneGeometry::PlaneGeometry(const float size): m_size(size)
{
	setVertices(std::vector{
		Vector3f{-0.5f * m_size, 0.0f, -0.5f * m_size},
		Vector3f{0.5f * m_size, 0.0f, -0.5f * m_size},
		Vector3f{-0.5f * m_size, 0.0f, 0.5f * m_size},
		Vector3f{0.5f * m_size, 0.0f, 0.5f * m_size}
	});
}
