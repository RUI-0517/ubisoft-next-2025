#include "stdafx.h"
#include "RenderScene.h"

#include "App/AppSettings.h"

Vector3f RenderScene::m_gravity{0, -9.81f, 0};

RenderScene::RenderScene(): m_accumulatedTime(0.0f)
{
}

RenderScene::~RenderScene() = default;

void RenderScene::Init()
{
	// Init physics world
	m_physicsWorld = std::make_unique<World>();
	m_physicsWorld->setGravity(m_gravity);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);

	// First Body
	// {
	// 	const auto& body = m_physicsWorld->createBody(1.0f);
	// 	if (!body)
	// 	{
	// 		throw std::runtime_error("Failed to create body");
	// 	}
	// 	body->transform.position = {-1.5f, 10.0f, -1.5f};
	// 	const auto& sphereGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
	// 	if (!sphereGeom)
	// 	{
	// 		throw std::runtime_error("Failed to create SphereGeometry");
	// 	}
	// 	sphereGeom->attachBody(body);
	// 	m_renderer->addRenderObject<SphereObject>(sphereGeom, RED);
	// }

	// // Second Body
	// {
	// 	const auto& body = m_physicsWorld->createBody(1.0f);
	// 	if (!body)
	// 	{
	// 		throw std::runtime_error("Failed to create body");
	// 	}
	// 	body->transform.position = {3.0f, 15.0f, 0.0f};
	// 	const auto& sphereGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
	// 	if (!sphereGeom)
	// 	{
	// 		throw std::runtime_error("Failed to create SphereGeometry");
	// 	}
	// 	sphereGeom->attachBody(body);
	// 	m_renderer->addRenderObject<SphereObject>(sphereGeom, GREEN);
	// }
	//
	// // Third Body
	// {
	// 	const auto& body = m_physicsWorld->createBody(1.0f);
	// 	if (!body)
	// 	{
	// 		throw std::runtime_error("Failed to create body");
	// 	}
	// 	body->transform.position = {-3.0f, 15.0f, 0.0f};
	// 	const auto& sphereGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
	// 	if (!sphereGeom)
	// 	{
	// 		throw std::runtime_error("Failed to create sphereGeom");
	// 	}
	// 	sphereGeom->attachBody(body);
	// 	m_renderer->addRenderObject<SphereObject>(sphereGeom, GREEN);
	// }

	constexpr size_t n = 2;
	constexpr float spacing = 3;
	constexpr float startX = -(static_cast<float>(n) - 1) * spacing / 2.0f;
	constexpr float startZ = -(static_cast<float>(n) - 1) * spacing / 2.0f;
	
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			const float mass = (i + j) % 2 == 0 ? 1.0f : 2.0f;
			const auto& body = m_physicsWorld->createBody(mass);
			if (!body) throw std::runtime_error("Failed to create body");
	
			body->transform.position = {startX + i * spacing, 10, startZ + j * spacing};
	
			const auto& sphereGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
			if (!sphereGeom) throw std::runtime_error("Failed to create SphereGeometry");
			sphereGeom->attachBody(body);
	
			MaterialId materialId = (i + j) % 2 == 0 ? RED : YELLOW;
			m_renderer->addRenderObject<SphereObject>(sphereGeom, materialId);
		}
	}
}

void RenderScene::Update(const float deltaTimeInSecond)
{
	m_accumulatedTime += deltaTimeInSecond;
	if (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_physicsWorld->simulate(m_fixedDeltaTime);
		m_accumulatedTime -= m_fixedDeltaTime;
	}

	update_camera(deltaTimeInSecond);

	m_renderer->update(deltaTimeInSecond);
}

void RenderScene::Render()
{
	m_renderer->Render();
}

void RenderScene::Shutdown()
{
}

void RenderScene::update_camera(const float deltaTimeInSecond) const
{
	Camera& camera = m_renderer->getCamera();
	constexpr float angularSpeed = 0.5f;

	static float currentAngle = 0.0f;
	currentAngle += angularSpeed * deltaTimeInSecond;

	constexpr float pi = PI;
	if (currentAngle > 2.0f * pi) currentAngle -= 2.0f * pi;

	constexpr float radius = 10.0f;
	camera.position.x = radius * sin(currentAngle);
	camera.position.z = radius * cos(currentAngle);
	camera.update();
}
