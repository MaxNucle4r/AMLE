#include "LegacyModel.h"





TextTypes::TransitionMap& LegacyModel::getTextTransitions()
{
	return TextTransitions;
}

TextTypes::ProbabilityMap& LegacyModel::getTextProbs()
{
	return TextProbs;
}


const TextTypes::TransitionMap& LegacyModel::getTextTransitions() const
{
	return TextTransitions;
}

const TextTypes::ProbabilityMap& LegacyModel::getTextProbs() const
{
	return TextProbs;
}



CharTypes::TransitionMap& LegacyModel::getCharTransitions()
{
	return CharTransitions;
}

CharTypes::ProbabilityMap& LegacyModel::getCharProbs()
{
	return CharProbs;
}


const CharTypes::TransitionMap& LegacyModel::getCharTransitions() const
{
	return CharTransitions;
}

const CharTypes::ProbabilityMap& LegacyModel::getCharProbs() const
{
	return CharProbs;
}





void LegacyModel::clearTextTransitions()
{
	TextTransitions.clear();
}

void LegacyModel::clearTextProbs()
{
	TextProbs.clear();
}

void LegacyModel::clearCharTransitions()
{
	CharTransitions.clear();
}

void LegacyModel::clearCharProbs()
{
	CharProbs.clear();
}



void LegacyModel::clear()
{
	clearCharProbs();
	clearCharTransitions();

	clearTextProbs();
	clearTextTransitions();
}





bool LegacyModel::isEmpty() const
{
	return TextTransitions.empty() &&
		CharTransitions.empty() &&
		TextProbs.empty() &&
		CharProbs.empty();

}