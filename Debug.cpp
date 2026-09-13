#include "Debug.h"





DebugFlags operator| (DebugFlags a, DebugFlags b)
{
	return static_cast<DebugFlags>
		(
			static_cast<DebugMask>(a)|
			static_cast<DebugMask>(b)
		);
}


DebugFlags operator& (DebugFlags a, DebugFlags b)
{
	return static_cast<DebugFlags>
		(
			static_cast<DebugMask>(a)&
			static_cast<DebugMask>(b)
		);
}


DebugFlags operator~ (DebugFlags flag)
{
	return static_cast<DebugFlags>
		(~static_cast<DebugMask>(flag));
}





void Debug::enable(DebugFlags flag)
{
	mask |= static_cast<DebugMask>(flag);
}


void Debug::disable(DebugFlags flag)
{
	mask &= ~static_cast<DebugMask>(flag);
}


bool Debug::enabled(DebugFlags flag) const
{
	return (mask & static_cast<DebugMask>(flag)) != 0;
}


void Debug::disableAll()
{
	mask &= ~static_cast<DebugMask>(DebugFlags::All);
}


void Debug::enableAll()
{
	mask |= static_cast<DebugMask>(DebugFlags::All);
}