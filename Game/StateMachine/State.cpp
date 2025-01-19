#include "pch.h"
#include "State.h"

struct State::Impl
{
	explicit Impl(const std::shared_ptr<State>& owner) : m_owner(owner)
	{
	}

	void init() const
	{
		m_owner->on_init();
	}

	void enter()
	{
		m_timeEllipse = 0.0f;
		m_owner->on_enter();
	}

	void update(const float deltaTimeInSecond)
	{
		m_timeEllipse += deltaTimeInSecond;
		m_owner->on_update(deltaTimeInSecond);
	}

	void render() const
	{
		m_owner->on_render();
	}

	void shutdown() const
	{
		m_owner->on_shutdown();
	}

	void addRelation(const std::shared_ptr<State>& nextState, const std::function<bool()>& condition)
	{
		m_nextStates.push_back(nextState);
		m_conditions.push_back(condition);
	}

	[[nodiscard]] const std::vector<std::function<bool()>>& getConditions() const
	{
		return m_conditions;
	}

	[[nodiscard]] const std::shared_ptr<State>& getState(const size_t index) const
	{
		return m_nextStates[index];
	}

	void setGraph(const std::shared_ptr<StateMachineGraph>& graph)
	{
		m_graph = graph;
	}

	const std::shared_ptr<StateMachineGraph>& getGraph()
	{
		return m_graph;
	}

	[[nodiscard]] float getTimeEllipse() const
	{
		return m_timeEllipse;
	}

	std::shared_ptr<StateMachineGraph> m_graph;
	std::shared_ptr<State> m_owner;
	std::vector<std::shared_ptr<State>> m_nextStates;
	std::vector<std::function<bool()>> m_conditions;
	float m_timeEllipse = 0.0f;
};

State::State(const std::shared_ptr<StateMachineGraph>& graph)
{
	pImpl = new Impl(std::shared_ptr<State>(this));
	pImpl->setGraph(graph);
}

State::~State()
{
	delete pImpl;
}

State::State(const State& other)
{
}

State::State(State&& other) noexcept: pImpl(other.pImpl)
{
}

State& State::operator=(const State& other)
{
	if (this == &other)
		return *this;
	pImpl = other.pImpl;
	return *this;
}

State& State::operator=(State&& other) noexcept
{
	if (this == &other)
		return *this;
	pImpl = other.pImpl;
	return *this;
}

void State::init() const
{
	pImpl->init();
}

void State::enter() const
{
	pImpl->enter();
}

void State::update(const float deltaTimeInSecond) const
{
	pImpl->update(deltaTimeInSecond);
}

void State::render() const
{
	pImpl->render();
}

void State::shutdown() const
{
	pImpl->shutdown();
}

void State::addTransition(const std::shared_ptr<State>& nextState, const std::function<bool()>& condition) const
{
	pImpl->addRelation(nextState, condition);
}

[[nodiscard]] const std::vector<std::function<bool()>>& State::getConditions() const
{
	return pImpl->getConditions();
}

[[nodiscard]] const std::shared_ptr<State>& State::getState(const size_t index) const
{
	return pImpl->getState(index);
}

void State::setGraph(const std::shared_ptr<StateMachineGraph>& graph) const
{
	pImpl->setGraph(graph);
}

const std::shared_ptr<StateMachineGraph>& State::getGraph() const
{
	return pImpl->getGraph();
}

float State::getTimeEllipse() const
{
	return pImpl->getTimeEllipse();
}

void State::on_init()
{
}

void State::on_enter()
{
}

void State::on_update(float deltaTimeInSecond)
{
}

void State::on_render()
{
}

void State::on_shutdown()
{
}
