#include "Configuration.h"





ModelType Configuration::getDefaultModel() const
{
    return defaultTextModel;
}


void Configuration::setDefaultModel(ModelType model)
{
    defaultTextModel = model;
}



size_t Configuration::getDefaultNGramSize() const
{
    return defaultNGramSize;
}


void Configuration::setDefaultNGramSize(size_t size)
{
    if (size < 2)
        throw std::invalid_argument("NGram size must be at least 2\n");

    if (size > 5)
        UI::Warning::printWarningText(
            "The higher the NGram size the more data required, "
            "THIS IS JUST A WARNING\n"
        );

    defaultNGramSize = size;
}



void Configuration::setTemp(double t)
{
    if (t <= 0.0)
        throw std::invalid_argument(
            "Temperature must be greater than 0"
        );

    else if (t > 2.5)
        throw std::invalid_argument(
            "Temperature must be smaller or equal to 2.5"
        );

    defaultTemperature = t;
}


double Configuration::getTemp() const
{
    return defaultTemperature;
}



bool Configuration::getAutoSave() const
{
    return autoSave;
}


void Configuration::setAutoSave(bool value)
{
    autoSave = value;
}



int Configuration::getMaxWords() const
{
    return maxWords;
}


void Configuration::setMaxWords(int w)
{
    // maybe tinker with this later
    if (w < 30)
        throw std::invalid_argument(
            "Max word count must be at least 30"
        );

    else if (w > 10000)
        UI::Warning::maxWordCount(w);

    maxWords = w;
}



int Configuration::getAutoSaveMinutes() const
{
    return autoSaveMinutes;
}


void Configuration::setAutoSaveMinutes(int m)
{
    if (m == 0)
    {
        UI::Warning::printWarningText(
            "Auto save timer cannot be 0, "
            "timer will be set to the default 5 minutes\n"
        );

        m = 5;
    }

    else if (m < 0)
    {
        UI::Warning::printWarningText(
            "Auto save timer cannot be smaller 0, "
            "timer will be set to the default 5 minutes\n"
        );

        m = 5;
    }

    else if (m > 15)
    {
        UI::Warning::printWarningText(
            "Auto save timer will be set to more than 15 minutes. "
            "THIS IS JUST A WARNING\n"
        );
    }

    autoSaveMinutes = m;
}