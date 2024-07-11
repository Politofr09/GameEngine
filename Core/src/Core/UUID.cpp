#include "UUID.h"

#include <random>

namespace Core
{

	// Random generation
	static std::random_device randomDevice;
	static std::mt19937 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	UUID::UUID()
		: m_UUID(uniformDistribution(engine))
	{
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}

}