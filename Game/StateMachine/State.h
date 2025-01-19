#pragma once
#include "StateMachine.h"
#include <functional>
#include <memory>

class StateMachineGraph;

class STATEMACHINE_API State
{
	struct Impl;

public:
	State(const std::shared_ptr<StateMachineGraph>& graph);
	virtual ~State();

	State(const State& other);

	State(State&& other) noexcept;

	State& operator=(const State& other);

	State& operator=(State&& other) noexcept;

	// Lifecycle API
	void init() const;
	void enter() const;
	void update(float deltaTimeInSecond) const;
	void render() const;
	void shutdown() const;

	// Add relation
	void addRelation(const std::shared_ptr<State>& nextState, const std::function<bool()>& condition) const;

	[[nodiscard]] const std::vector<std::function<bool()>>& getConditions() const;
	[[nodiscard]] const std::shared_ptr<State>& getState(size_t index) const;

	void setGraph(const std::shared_ptr<StateMachineGraph>& graph) const;
	[[nodiscard]] const std::shared_ptr<StateMachineGraph>& getGraph() const;
	[[nodiscard]] float getTimeEllipse() const;

protected:
	virtual void on_init();

	virtual void on_enter();

	virtual void on_update(float deltaTimeInSecond);

	virtual void on_render();

	virtual void on_shutdown();

private:
	Impl* pImpl;
};
