#include "stdafx.h"
#include "PhysicsScene.h"

#include "App/app.h"
#include "App/AppSettings.h"
#include <sstream>

Vector3f PhysicsScene::m_gravity{0, -9.81f, 0};

PhysicsScene::PhysicsScene(): m_accumulatedTime(0.0f)
{
}

PhysicsScene::~PhysicsScene() = default;

void PhysicsScene::Init()
{
	// Init physics world
	m_physicsWorld = std::make_unique<World>();
	m_physicsWorld->setGravity(m_gravity);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);

	add_object();
}

void PhysicsScene::Update(const float deltaTimeInSecond)
{
	m_accumulatedTime += deltaTimeInSecond;
	if (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_physicsWorld->simulate(m_fixedDeltaTime);
		m_accumulatedTime -= m_fixedDeltaTime;
	}

	m_renderer->update();
}

void PhysicsScene::Render()
{
	m_renderer->Render();

	const Vector3f a{static_cast<float>(APP_INIT_WINDOW_WIDTH), static_cast<float>(APP_INIT_WINDOW_HEIGHT), 0.0f};
	const Vector3f b{0.0f, 0.0f, 50.0f};
	const Vector3f result = (a + b) / 2.0f;

	// App::Print(result.x - 10, result.y - 10, "+");

	const std::shared_ptr<Body>& sphereBody = m_physicsWorld->bodies[1];
	const Vector3f& velocity = sphereBody->getLinearVelocity();
	const Vector3f& position = sphereBody->transform.position;

	std::ostringstream os;
	os << "Velocity: " << velocity;
	App::Print(result.x - 10, result.y - 10, os.str().c_str(), 0, 0, 0);
	os.str("");
	os.clear();
	os << "Position: " << position;
	App::Print(result.x - 10, result.y - 50, os.str().c_str(), 0, 0, 0);
}

void PhysicsScene::Shutdown()
{
}

void PhysicsScene::add_object()
{
	m_physicsWorld->bodies.emplace_back(std::make_shared<Body>(1.0f));
	m_physicsWorld->bodies[1]->transform.position = {0.0f, 10.0f, 0.0f};

	m_physicsWorld->sphereGeom = std::make_shared<SphereGeometry>(1.0f);
	m_physicsWorld->sphereGeom->attachBody(m_physicsWorld->bodies[1]);

	m_physicsWorld->geometries.push_back(m_physicsWorld->sphereGeom);
	m_renderer->addObject<SphereObject>(m_physicsWorld->sphereGeom, 1.0f);
}
