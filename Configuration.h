#pragma once
#include <stdexcept>
#include <limits>


#include "UI.h"
#include "ModelType.h"





class Configuration
{
private:


    // Generation
    double defaultTemperature = 1.0;
    int maxWords = 100;


    ModelType defaultTextModel = ModelType::Legacy;
    size_t defaultNGramSize = 3;




    // UI
    bool coloredOutput = false;
    bool showTimings = false;

    // Runtime
    bool autoSave = false;
    int autoSaveMinutes = 5;




public:

    ModelType getDefaultModel() const;
    void setDefaultModel(ModelType model);


    size_t getDefaultNGramSize() const;
    void setDefaultNGramSize(size_t size);


    void setTemp(double t);
    double getTemp() const;


    bool getAutoSave() const;
    void setAutoSave(bool value);


    int getMaxWords() const;
    void setMaxWords(int w);


    int getAutoSaveMinutes() const;
    void setAutoSaveMinutes(int m);


};

