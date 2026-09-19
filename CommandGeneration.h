#pragma once
#include <string>
#include <optional>
#include <vector>

#include "Types.h"
#include "ModelType.h"
#include "Debug.h"
#include "Commands.h"
#include "Vocabulary.h"
#include "Generation.h"





namespace Generation::GenerationHelpers
{

	ModelType pickModelType(const ModelType& defaultModel,
		const ParsedCommand& cmd);

	std::optional<std::vector<std::string>> pickGenerationSeed(const ParsedCommand& cmd,
		const Debug& debugger);

	size_t pickModelTypeSize(const ParsedCommand& cmd, const size_t defaultSize);

	void handleGenerationDebug(const Debug& debugger, const std::string& reply,
		const EndingType& endtype, VocabularyManager& vm);



}

namespace Execute::Generate
{

	void Text(const ParsedCommand& cmd, AIContext& ctx);

	void Word(const ParsedCommand& cmd, AIContext& ctx);

}
