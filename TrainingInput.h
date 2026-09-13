#pragma once
#include <chrono>





class TrainingInput
{
private:

	std::string normalized;
	std::string original;

	int timesSeen = 1;
	
	std::chrono::system_clock::time_point firstSeen;
	std::chrono::system_clock::time_point lastSeen;



public:

	TrainingInput(std::string originalText,
		std::string normalizedText)
		:
		original(std::move(originalText)),
		normalized(std::move(normalizedText))
	{
		firstSeen = std::chrono::system_clock::now();
		lastSeen = firstSeen;
	}



	const std::string& getNormalized() const;
	const std::string& getOriginal() const;


	int getTimesSeen() const;


	void incrementSeen();


	std::chrono::system_clock::time_point getFirstSeen() const;
	std::chrono::system_clock::time_point getLastSeen() const;


	void updateLastSeen();



};

