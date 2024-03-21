#include <unordered_map>

#pragma once

class ComponentStorageBase
{
public:
	virtual ~ComponentStorageBase() = default;
};

template <typename T>
class ComponentStorage : public ComponentStorageBase
{
private:
	std::unordered_map<std::size_t, T> _components;

public:
	bool HasEntity(std::size_t entity)
	{
		return _components.find(entity) != _components.end();
	}

	void AddComponent(std::size_t entity, const T& component)
	{
		_components[entity] = component;
	}

	void RemoveComponent(std::size_t entity)
	{
		_components.erase(entity);
	}

	T& GetComponent(std::size_t entity)
	{
		return _components.at(entity);
	}

	~ComponentStorage() override
	{
	};
};