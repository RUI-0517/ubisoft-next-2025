#pragma once
#include "Body.h"

class Geometry
{
	std::shared_ptr<Body> m_body;

protected:
	std::vector<Vector3f> m_vertices;

public:
	virtual ~Geometry() = default;

	virtual Vector3f GetSupportPoint(const Vector3f& direction);

	[[nodiscard]] const std::shared_ptr<Body>& getBody() const;
	void attachBody(const std::shared_ptr<Body>& body);
};
