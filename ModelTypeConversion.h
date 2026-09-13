#pragma once

#include <string>
#include <string_view>


#include "ModelType.h"





//namespace ModelAndString
namespace ModelTypeUtils
{

	inline ModelType stringToModelType(std::string_view stringModel)
	{


		if (stringModel == "ngram")
			return ModelType::NGram;


		else if (stringModel == "legacy")
			return ModelType::Legacy;


		else
			return ModelType::NONE;
	}


	inline std::string modelTypeToString(ModelType model)
	{
		switch (model)
		{
		case ModelType::Legacy:
			return "legacy";


		case ModelType::NGram:
			return "ngram";


		default:
			return "";
		}

	}



}





