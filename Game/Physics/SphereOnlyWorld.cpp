#include "pch.h"
#include "SphereOnlyWorld.h"

void SphereOnlyWorld::simulate(const float timeStep)
{
	m_timeStep = timeStep;

	for (const auto& body : m_bodies)
	{
		if (body->isKinematic()) continue;

		constexpr float frictionCoefficient = 0.99f;

		// Forward Euler
		Vector3f acceleration = body->calculateAcceleration();
		acceleration += m_gravity;

		Vector3f& velocity = body->getLinearVelocity();
		velocity += acceleration * timeStep;
		velocity *= frictionCoefficient;

		Vector3f& position = body->transform.position;
		position += velocity * timeStep;

		// Angular Velocity
		Vector3f angularAcceleration = body->calculateAngularAcceleration();
		Vector3f& angularVelocity = body->getAngularVelocity();
		angularVelocity += angularAcceleration * timeStep;
		angularVelocity *= frictionCoefficient;

		Vector4f angularVelocityQuaternion = angularVelocity;
		angularVelocityQuaternion.w = 0.0f;

		Quaternion& rotation = body->transform.rotation;
		rotation += 0.5f * angularVelocityQuaternion.rotate(rotation * timeStep);

		body->resetAccumulatedForce();
		body->resetAccumulatedTorque();
	}

	handle_collision();
}

void SphereOnlyWorld::setGravity(const Vector3f& gravity)
{
	m_gravity = gravity;
}

std::shared_ptr<Body> SphereOnlyWorld::createBody()
{
	auto object = std::make_shared<Body>();
	m_bodies.emplace_back(object);
	return object;
}

std::shared_ptr<Body> SphereOnlyWorld::createBody(float mass)
{
	auto object = std::make_shared<Body>(mass);
	m_bodies.emplace_back(object);
	return object;
}

const std::vector<std::shared_ptr<Body>>& SphereOnlyWorld::getBodies() const
{
	return m_bodies;
}

const std::vector<std::shared_ptr<SphereGeometry>>& SphereOnlyWorld::getGeometries() const
{
	return m_sphereGeometries;
}

void SphereOnlyWorld::handle_collision() const
{
	const size_t size = m_sphereGeometries.size();
	for (size_t i = 0; i < size; ++i)
	{
		const std::shared_ptr<SphereGeometry> self = m_sphereGeometries[i];

		for (size_t j = i + 1; j < size; ++j)
		{
			const std::shared_ptr<SphereGeometry> other = m_sphereGeometries[j];

			const Vector3f& selfCenter = self->getTransform().position;
			const Vector3f& otherCenter = other->getTransform().position;

			const float selfRadius = self->getRadius();
			const float otherRadius = other->getRadius();

			const float distance = selfCenter.distance(otherCenter);

			const bool collided = distance < (selfRadius + otherRadius);
			if (!collided) continue;

			const Vector3f normal = (otherCenter - selfCenter).normalize();

			const Vector3f pa = selfCenter + normal * selfRadius;
			const Vector3f pb = otherCenter - normal * otherRadius;

			const Vector3f ra = pa - selfCenter;
			const Vector3f rb = pb - otherCenter;

			const float c = (pb - pa).dot(normal);

			const CollisionInfo collisionInfo{normal, ra, rb, c};
			solve_penetration_constraint(collisionInfo, self->getBody(), other->getBody());
		}
	}
}

// See https://danielchappuis.ch/download/ConstraintsDerivationRigidBody3D.pdf p.g.7
void SphereOnlyWorld::solve_penetration_constraint(const CollisionInfo& collision, const std::shared_ptr<Body>& self,
                                                   const std::shared_ptr<Body>& other) const
{
	// Jacobian Matrix Construction
	// [-n_1, -(r_1 X n_1), n_1, (r_2 X n_1)]
	// -n_1
	const Vector3f j0 = -collision.normal;
	// -(r_1 X n_1)
	const Vector3f j1 = -collision.ra.cross(collision.normal);
	// n_1
	const Vector3f j2 = collision.normal;
	// r_2 X n_1
	const Vector3f j3 = collision.rb.cross(collision.normal);

	// Rigidbody Info
	const float selfMassInv = self->getMassInv();
	const float selfInertiaTensorInv = self->getInertiaTensorInv();
	const float otherMassInv = other->getMassInv();
	const float otherInertiaTensorInv = other->getInertiaTensorInv();

	// Current velocities of the rigid bodies
	Vector3f& selfLinearVelocity = self->getLinearVelocity();
	Vector3f& selfAngularVelocity = self->getAngularVelocity();
	Vector3f& otherLinearVelocity = other->getLinearVelocity();
	Vector3f& otherAngularVelocity = other->getAngularVelocity();

	// Calculate the effective mass (1 / (J M^-1 J^T))
	// Represents how mass and inertia affect the system's response to impulses
	const float k = selfMassInv + j1.dot(selfInertiaTensorInv * j1) +
		otherMassInv + j3.dot(otherInertiaTensorInv * j3);
	const float effectiveMass = 1 / k;

	// Compute the Jacobian-velocity product (Jv)
	// Represents the relative velocity projected onto the constraint space
	const float jv = j0.dot(selfLinearVelocity) + j1.dot(selfAngularVelocity) +
		j2.dot(otherLinearVelocity) + j3.dot(otherAngularVelocity);

	// Baumgarte Stabilization
	// b = -beta/dt * c
	// TODO: Using physics material
	constexpr float beta = 0.25f;

	// Restitution (bounciness)
	// restitution * (-v_a (-w_a X r_a) + v_b + (w_b X r_b))
	// Controls the amount of energy preserved after the collision
	// TODO: Using physics material
	constexpr float restitution = 0.8f;
	
	const Vector3f relativeVelocity = -selfLinearVelocity - selfAngularVelocity.cross(collision.ra)
		+ otherLinearVelocity + otherAngularVelocity.cross(collision.rb);
	const float closingVelocity = relativeVelocity.dot(collision.normal);

	const float b = (-beta / m_timeStep) * collision.c + restitution * closingVelocity;

	// Calculate the Lagrange multiplier (impulse magnitude)
	const float lambda = effectiveMass * (-jv + b);

	if (!self->isKinematic())
	{
		selfLinearVelocity += selfMassInv * lambda * j0;
		selfAngularVelocity += selfInertiaTensorInv * lambda * j1;
	}

	if (!other->isKinematic())
	{
		otherLinearVelocity += otherMassInv * lambda * j2;
		otherAngularVelocity += otherInertiaTensorInv * lambda * j3;
	}
}
