#include "System.h"

inline void System::Tick()
{
	if (_function)
	{
		_function(_world);
	}
}

inline const std::string& System::GetName()
{
	return _name;
}

inline int System::GetPriority()
{
	return _priority;
}
