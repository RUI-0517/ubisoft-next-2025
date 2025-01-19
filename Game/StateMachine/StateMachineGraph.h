#pragma once
#include "StateMachine.h"
#include <memory>

class State;
class STATEMACHINE_API StateMachineGraph
{
	struct Impl;
	Impl* pImpl;

public:
	StateMachineGraph();
	virtual ~StateMachineGraph();

	StateMachineGraph(const StateMachineGraph& other);
	StateMachineGraph(StateMachineGraph&& other) noexcept;
	StateMachineGraph& operator=(const StateMachineGraph& other);
	StateMachineGraph& operator=(StateMachineGraph&& other) noexcept;

	void addState(const std::shared_ptr<State>& state) const;
	void switchState(size_t nextIndex) const;

	// Lifecycle API
	void init() const;
	void update(float deltaTimeInSecond) const;
	void render() const;
	void shutdown() const;

	virtual void on_init() ;
	virtual void on_update(float deltaTimeInSecond) ;
	virtual void on_render() ;
	virtual void on_shutdown() ;
};
