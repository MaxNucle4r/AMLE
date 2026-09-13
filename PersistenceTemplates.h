#pragma once

#include <vector>


#include "Persistence.h"




// std::false_type is basically a standard-library type that says:
// value = false
// Everything is NOT a pair/vector unless it is specifically told that it is.

template<typename T>
struct is_pair : std::false_type {};

template<typename A, typename B>
struct is_pair<std::pair<A, B>> : std::true_type {};

template<typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;



template<typename T>
struct is_vector : std::false_type {};

template<typename T>
struct is_vector<std::vector<T>> : std::true_type {};

template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;





namespace Persistence::Detail
{
	template<typename Context>
	json serializeContext(const Context& context)
	{
		if constexpr (is_pair_v<Context>)
		{
			return
			{
				context.first,
				context.second
			};
		}


		else if constexpr (is_vector_v<Context>)
		{
			json result = json::array();

			for (const auto& element : context)
			{
				result.push_back(element);
			}


			return result;

		}


		// static_assert checks while the program is compiling
		else static_assert(
			is_pair_v<Context> || is_vector_v<Context>,
			"serializeContext() only supports std::pair or std::vector"
			);



	}



	template<typename Context>
	Context deserializeContext(const json& jsonContext)
	{
		if constexpr (is_pair_v <Context> )
		{
			return Context
			{
				jsonContext[0].template get<typename Context::first_type>(),
				jsonContext[1].template get<typename Context::second_type>()

			};

		}


		else if constexpr (is_vector_v<Context>)
		{
			Context result;

			for (const auto& element : jsonContext)
			{
				result.push_back(element.template get<typename Context::value_type>());
			}


			return result;

		}


		else
		{
			static_assert(
				is_pair_v<Context> || is_vector_v<Context>,
				"deserializeContext() only supports std::pair or std::vector"
				);
		}




	}
	




	template <typename Map>
	void saveTransitions(json& j, const Map& map)
	{
		j = json::array();

		using Next = typename Map::mapped_type::key_type;


		for (const auto& [context, nextMap] : map)
		{
			json transition;

			transition["context"] = serializeContext(context);

			transition["next"] = json::object();


			for (const auto& [nextToken, count] : nextMap)
			{

				if constexpr (std::is_same_v<Next, char>)
				{
					transition["next"][std::string(1, nextToken)] = count;
				}

				else
				{
					transition["next"][nextToken] = count;
				}
				
			}


			j.push_back(transition);

		}

	}



	 
	template <typename Map>
	void loadTransitions(const json& j, Map& map)
	{
		map.clear();

		using Context = typename Map::key_type;
		using Next = typename Map::mapped_type::key_type;

		for (const auto& transition : j)
		{
		
			Context context =
				deserializeContext<Context>(transition["context"]);


			for (const auto& [next, count] : transition["next"].items())
			{

				if constexpr (std::is_same_v<Next, char>)
				{
					map[context][next[0]] = count.get<int>();
				}

				else
				{
					map[context][next] = count.get<int>();
				}
				
			}


		}


	}


}