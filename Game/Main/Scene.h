#pragma once
class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;
	Scene(Scene&& scene) noexcept = delete;
	Scene& operator=(Scene&& scene) noexcept = delete;
	virtual void Init() = 0;
	virtual void Update(float deltaTimeInSecond) = 0;
	virtual void Render() = 0;
	virtual void Shutdown() = 0;
};
