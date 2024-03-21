#include <functional>
#include <string>

#pragma once

// Forward declaration
class World;

class System
{
public:
	System(World& w, const std::string& name = "", int priority = 0) : _world(w), _name(name), _priority(priority) {}

	System(World& w, std::function<void(World&)> func, const std::string& name = "", int priority = 0) : _world(w), _function(func), _name(name), _priority(priority) {}

	virtual void Tick();

	inline const std::string& GetName();

	inline int GetPriority();

protected:
	World& _world;

private:
	int _priority;
	const std::string _name;
	std::function<void(World&)> _function;
};