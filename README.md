# AMLE — Adaptive Markov Language Engine

**Version:** 0.2.3

A personal C++ project that started as “I want to build the dumbest possible AI over the summer” and got out of hand (in a good way).

AMLE is a console-based **Markov chain / n-gram language model engine**.  
It supports both a legacy bigram-style model and configurable n-gram models (size 2–7), training, generation with temperature, vocabulary tracking, memory of past training data, persistence to `.amle` files, and a custom command language.

This is **not** a polished product. It is a living personal experiment that is still rough around the edges.

---



## Features

- Legacy (pair-based) + general n-gram models (word-level and character-level)
- Interactive training from text input
- Text and word generation with temperature and optional seed
- Model save / load (JSON-based `.amle` files with metadata)
- Vocabulary tracking (input vs generated counts)
- Session memory
- Custom command syntax: `@command>target{option = value}`
- Debug flags, configuration system, and a growing set of tools
- Clean modular command system (Command Executor is now a thin orchestrator)

---



## Requirements

### To build from source

- **Windows** (primary target)
- **Visual Studio 2022** or newer (tested with VS 2026)
- **C++23** (or later)
- **NuGet package**: `nlohmann.json` (version 3.12.0 is used)

The project uses a Visual Studio solution (`.slnx` / `.vcxproj`).

### To run a pre-built executable

In theory a Release `.exe` can be run standalone if:
- You have the matching **Microsoft Visual C++ Redistributable** installed
- You run it from a folder that has write access (it creates a `models/` directory)
- No other external DLLs are required (nlohmann/json is header-only)

There is currently **no official release binary**. You will need to build it yourself for now.

---



## Building

1. Clone the repository  
   ```bash
   git clone https://github.com/MaxNucle4r/AMLE.git
   cd AMLE

   Open AMLE.slnx (or the .vcxproj) in Visual Studio.
2. Restore NuGet packages (Visual Studio usually does this automatically).
  
4. If needed: right-click the solution → Restore NuGet Packages.

5. Set the configuration to Release (or Debug) and build (Ctrl+Shift+B).

6. Run the program (F5 or Ctrl+F5).

The executable will appear in the normal Visual Studio output folders (x64/Release, etc.).




## *BASIC USAGE*
@command>target{option1=value1/option2=value2}


**EXAMPLES**

@help                     
@version
@cls

@train>input                   # start interactive training
@generate>text
@generate>text{temperature = 1.2 / model = ngram / ngram-size = 4 / seed = hello world}
@generate>word{temperature = 0.8}

@print>transitions
@print>probs{model = ngram / ngram-size = 3}
@print>memory
@print>vocab

@model>save{filename = my_model}
@model>load{filename = my_model}
@model>files

@config>...
@debug>...
@clear>...


#########
***RUNNING @help*** will provide all the necessary info
#########



## Project Structure (high level)

AMLE.cpp — entry point
AMLE Loop.* — main interactive loop
AIContext.* — central state (models, vocabulary, memory, config, debug…)
Command Executor.* — thin router
Command*.h/.cpp — one subsystem per command group (Generate, Print, Training, Persistence, Config, Debug, Clear, Help…)
Generation.*, Training.*, NGram.*, LegacyModel.*, Model.* — core engine
Persistence.* — save/load .amle files
Commands.h — command/target/option registry (the heart of the custom language)




## **Notes & Status**
This is a personal learning project. Code quality varies because it evolved organically.
Many features are still incomplete or marked in TO_DO.h.
The command language and option system are under active improvement.
Windows-only for now (cls, paths, Visual Studio project files).
No unit tests yet.
Models are ignored by git (see .gitignore).





## **Credits**
Built with a lot of stubbornness, curiosity, and help from ChatGPT along the way.
Feel free to look around, break things, or steal ideas.
If you actually use it and find bugs or have questions, open an issue.






## Important Notes & Current Limitations

### Saving System
**What is currently saved:**
- Legacy model (text + character transitions)
- All N-Gram models (text + character transitions for every size)
- Metadata (version, timestamps, statistics)

**What is NOT saved yet:**
- **Memory** (conversation / interaction history)
- **Vocabulary** (word counts and tracking)
**THIS WAS DUE TO THE MULTIPLE REMAKES OF THE PERSISTENCE SYSTEM**

The helper functions for saving/loading Memory and Vocabulary already exist in the code  
(`Persistence::MemorySL::...` and `Persistence::VocabularySL::...`),  
but they are currently **not being called**.

To enable them, you only need to call the existing functions inside  
`Persistence::save()` and `Persistence::load()`.

---

### General Status
- This is a personal learning project that evolved a lot over time.
- Many systems exist and work, but some are incomplete or not fully wired up.
- The command language, generation options, and UI are still under active development.
- Code quality varies because large parts were written while learning on the go.
- Windows + Visual Studio is the only officially tested environment.
