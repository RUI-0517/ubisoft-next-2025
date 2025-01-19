#include "pch.h"
#include "StateMachineGraph.h"
#include <stdexcept>

struct StateMachineGraph::Impl
{
	explicit Impl(const std::shared_ptr<StateMachineGraph>& owner) : m_owner(owner)
	{
	}

	void init() const
	{
		m_owner->on_init();
		for (const auto& state : m_states)
			state->init();
	}

	void update(const float deltaTimeInSecond)
	{
		m_owner->on_update(deltaTimeInSecond);

		const auto& currentState = m_states[m_currentStateIndex];
		// debounce key
		if (currentState->getTimeEllipse() > 0.5f)
		{
			const auto& currentConditions = currentState->getConditions();
			for (size_t i = 0; i < currentConditions.size(); ++i)
			{
				const std::function<bool()>& currentCondition = currentConditions[i];
				const bool shouldSwitch = currentCondition();

				if (shouldSwitch)
				{
					const auto& nextState = currentState->getState(i);
					const auto it = std::find(m_states.begin(), m_states.end(), nextState);

					if (it != m_states.end())
					{
						const size_t nextStateIndex = static_cast<size_t>(it - m_states.begin());
						switchState(nextStateIndex);
					}
					else throw std::runtime_error("State not found!");

					break;
				}
			}
		}

		currentState->update(deltaTimeInSecond);
	}

	void render() const
	{
		m_owner->on_render();
		const auto& currentState = m_states[m_currentStateIndex];
		currentState->render();
	}

	void shutdown() const
	{
		m_owner->on_shutdown();
		const auto& currentState = m_states[m_currentStateIndex];
		currentState->shutdown();
	}

	void addState(const std::shared_ptr<State>& state)
	{
		m_states.push_back(state);
	}

	void switchState(const size_t nextIndex)
	{
		const auto& currentState = m_states[m_currentStateIndex];
		if (currentState != nullptr) currentState->shutdown();
		m_currentStateIndex = nextIndex;
		m_states[m_currentStateIndex]->enter();
	}

private:
	std::vector<std::shared_ptr<State>> m_states;
	size_t m_currentStateIndex = 0;
	std::shared_ptr<StateMachineGraph> m_owner;
};

StateMachineGraph::StateMachineGraph()
{
	pImpl = new Impl(std::shared_ptr<StateMachineGraph>(this));
}

::StateMachineGraph::~StateMachineGraph()
{
	delete pImpl;
}

StateMachineGraph::StateMachineGraph(const StateMachineGraph& other) = default;

StateMachineGraph::StateMachineGraph(StateMachineGraph&& other) noexcept: pImpl(other.pImpl)
{
}

StateMachineGraph& StateMachineGraph::operator=(const StateMachineGraph& other)
{
	if (this == &other)
		return *this;
	pImpl = other.pImpl;
	return *this;
}

StateMachineGraph& StateMachineGraph::operator=(StateMachineGraph&& other) noexcept
{
	if (this == &other)
		return *this;
	pImpl = other.pImpl;
	return *this;
}

void StateMachineGraph::addState(const std::shared_ptr<State>& state) const
{
	pImpl->addState(state);
}

void StateMachineGraph::switchState(const size_t nextIndex) const
{
	pImpl->switchState(nextIndex);
}

void StateMachineGraph::init() const
{
	pImpl->init();
}

void StateMachineGraph::update(const float deltaTimeInSecond) const
{
	pImpl->update(deltaTimeInSecond);
}

void StateMachineGraph::render() const
{
	pImpl->render();
}

void StateMachineGraph::shutdown() const
{
	pImpl->shutdown();
}

void StateMachineGraph::on_init()
{
}

void StateMachineGraph::on_update(float deltaTimeInSecond)
{
}

void StateMachineGraph::on_render()
{
}

void StateMachineGraph::on_shutdown()
{
}
