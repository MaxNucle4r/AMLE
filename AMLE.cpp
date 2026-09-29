#include <iostream>
#include <string>
#include <algorithm>

#include "AIContext.h"
#include "CommandParser.h"
#include "AMLE Loop.h"










// -------------------- A M L E --------------------
// CHAT GPT CALLS THIS AMLE
// sounds actually cool
// short for ***Adaptive Markov Language Engine***
// he also suggested some funny ones: Word Goblin, Sentence Gremlin, AlmostAI, FakeGPT(xDDDDDD), The Yapper Engine
// but also some interesitng: EchoMind(hmm?), ThoughtChain, ProtoMind( hmm?), SeedThing, NeuroGlyph
// chat gpt is cooking :P
// --------------------------------------------------




//This little thing... is called AMLE... short for Adaptive Markov Language Engine
//A little trigram... that may or may not have gotten out of hand
//This isnt exactly jarvis 2.0... yet.
//But it is a start...
//Hella weird math stuff...
//Not 100% understand the math behind all this why exactly it is as it is...
//But... better to make it than to think of making it
//Huge credits to ChatGPT







int main()
{


    AIContext Amle;

    CommandParser Parser;



    AMLE::Run(Amle, Parser);


    return 0;
}

