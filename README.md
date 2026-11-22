AMLE — Adaptive Markov Language Engine

Version: "0.2.3"

A personal C++ project that started as “I want to build the dumbest possible AI over the summer” and got out of hand (in a good way).

AMLE is a console-based Markov chain / n-gram language model engine. It supports both a legacy bigram-style model and configurable n-gram models (sizes 2–7), training, text generation with temperature, vocabulary tracking, session memory, persistence to ".amle" files, and a custom command language.

This is not a polished product. It is a personal experiment that is still evolving and has plenty of rough edges.

---

Features

- Legacy pair-based model and general n-gram models (word-level and character-level).
- Interactive training from text input.
- Text and word generation with configurable temperature and optional seed.
- Model saving and loading through JSON-based ".amle" files with metadata.
- Vocabulary tracking (input vs. generated counts).
- Session memory.
- Custom command syntax: "@command>target{option=value/option=value}".
- Debug flags, configuration system, and a growing set of tools.
- Modular command system, with the Command Executor acting as a thin orchestrator.

---

Requirements

Building from source

- Operating system: Windows (primary target).
- IDE: Visual Studio 2022 or newer (tested with Visual Studio 2026).
- Language standard: C++23 or later.
- Dependency: "nlohmann.json" (version "3.12.0").

The project uses a Visual Studio solution (".slnx") and project file (".vcxproj").

Running a pre-built executable

In theory, a Release executable can run independently, provided that:

- The matching Microsoft Visual C++ Redistributable is installed.
- The executable is run from a directory with write access, since AMLE creates a "models/" directory.
- Any required runtime dependencies are available.

"nlohmann/json" is header-only, so it does not require a separate runtime DLL.

There is currently no official release binary. You will need to build the project yourself.

---

Building

1. Clone the repository:
   
   git clone https://github.com/MaxNucle4r/AMLE.git
cd AMLE

2. Open "AMLE.slnx" (or the ".vcxproj" file) in Visual Studio.

3. Restore the NuGet packages. Visual Studio usually does this automatically. If it does not, right-click the solution and select Restore NuGet Packages.

4. Select the "Release" or "Debug" configuration.

5. Build the project using Ctrl + Shift + B.

6. Run it using F5 (with debugging) or Ctrl + F5 (without debugging).

The executable will be generated in the usual Visual Studio output directory, such as "x64/Release".

---

Basic Usage

AMLE has its own command interface. Commands follow this general structure:

@command>target{option1=value1/option2=value2}

Examples

General commands

@help
@version
@cls

Training and generation

@train>input

@generate>text
@generate>text{temperature=1.2/model=ngram/ngram-size=4/seed=hello world}
@generate>word{temperature=0.8}

Printing information

@print>transitions
@print>probs{model=ngram/ngram-size=3}
@print>memory
@print>vocab

Model management

@model>save{filename=my_model}
@model>load{filename=my_model}
@model>files

Configuration and debugging

@config>...
@debug>...
@clear>...

«Tip: Running "@help" inside AMLE displays the available commands, targets, options, and their descriptions.»

---

Project Structure

High-level overview of the project:

Component| Responsibility
"AMLE.cpp"| Application entry point
"AMLE Loop.*"| Main interactive loop
"AIContext.*"| Central application state: models, vocabulary, memory, configuration, debug settings, etc.
"Command Executor.*"| Routes parsed commands to the appropriate subsystem
"Command*.h/.cpp"| Individual command subsystems (Generate, Print, Training, Model, Config, Debug, Clear, Help, etc.)
"Generation.*", "Training.*"| Text generation and model training
"NGram.*", "LegacyModel.*", "Model.*"| Language model implementations
"Persistence.*"| Saving and loading ".amle" files
"Commands.h"| Command, target, and option registry — the foundation of the custom command language

---

Notes & Status

AMLE is a personal learning project. Its codebase evolved organically, and code quality varies as a result.

- Many features are still incomplete or marked in "TO_DO.h".
- The command language and option system are under active development.
- The project is currently Windows-only (console commands, paths, and Visual Studio project files).
- There are no unit tests yet.
- Model files are excluded from Git through ".gitignore".

---

Important Notes & Current Limitations

Saving System

Currently saved:

- Legacy model (text and character transitions).
- All n-gram models (text and character transitions for every supported size).
- Metadata (version, timestamps, and statistics).

Not currently saved:

- Session memory (conversation / interaction history).
- Vocabulary (word counts and tracking).

This is a consequence of the multiple rewrites of the persistence system.

The helper functions for saving and loading Memory and Vocabulary already exist in the code:

- "Persistence::MemorySL::..."
- "Persistence::VocabularySL::..."

However, these functions are not currently called.

To enable saving and loading for these systems, the existing helper functions need to be integrated into "Persistence::save()" and "Persistence::load()".

General Limitations

- Some systems are implemented and functional, while others are incomplete or not fully integrated.
- Generation options and the user interface are still evolving.
- The project has only been tested on Windows with Visual Studio.

---

Credits

Built with a lot of stubbornness, curiosity, and help from ChatGPT along the way.

Feel free to look around, break things, or steal ideas.

If you use AMLE and find bugs or have questions, open an issue.