#pragma once
#include <iostream>

#include "Version.h"





constexpr inline void printVersions()
{
	std::cout << "AMLE version: " << Version::AMLE << "\n";
	std::cout << "Save format: " << Version::SaveFormat << "\n";
}




