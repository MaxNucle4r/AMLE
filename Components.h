#pragma once
#include <string_view>

#include "AMLE_ASSERT.h"






enum class ComponentID
{
	NONE, 

	AI_CONTEXT,
	LEGACY_MODEL,
	NGRAM_MODEL,

	DEBUGGER,

	VOCABULARY,

	MEMORY,

	CONFIGURATION

};


inline std::string_view ComponentIDToString(const ComponentID& id)
{
	switch (id)
	{
		case ComponentID::NONE:				return "None";

		case ComponentID::AI_CONTEXT:		return "AIContext";
		case ComponentID::LEGACY_MODEL:		return "Legacy";
		case ComponentID::NGRAM_MODEL:		return "Ngram";

		case ComponentID::DEBUGGER:			return "Debugger";
		case ComponentID::VOCABULARY:		return "Vocabulary";
		case ComponentID::MEMORY:			return "Memory";

		case ComponentID::CONFIGURATION:	return "Configuration";

		

		default:
			AMLE_ASSERT(false);
	}
}
