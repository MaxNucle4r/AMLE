#include "TrainingInput.h"





int TrainingInput::getTimesSeen() const
{
	return timesSeen;
}


void TrainingInput::incrementSeen()
{
	++timesSeen;
	lastSeen = std::chrono::system_clock::now();
}



std::chrono::system_clock::time_point TrainingInput::getFirstSeen() const
{
	return firstSeen;
}


std::chrono::system_clock::time_point TrainingInput::getLastSeen() const
{
	return lastSeen;
}


void TrainingInput::updateLastSeen()
{
	lastSeen = std::chrono::system_clock::now();
}



const std::string& TrainingInput::getNormalized() const
{
	return normalized;
}


const std::string& TrainingInput::getOriginal() const
{
	return original;
}

