#pragma once

#include <stdint.h>

namespace Core
{

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
		bool operator ==(const UUID& other) { return m_UUID == other.m_UUID; }

	private:
		uint64_t m_UUID;
	};

}