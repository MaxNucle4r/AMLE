#pragma once
#include <string_view>
#include <iostream>





namespace Validation
{
	template<typename Value>
	Value clampWithWarning(
		std::string_view name,
		Value current,
		Value min,
		Value max
		)
	{
        if (current < min)
        {
            std::cout << "\n\n\n";
            std::cout << name << " too low. Using minimum value.\n";
            std::cout << "\n\n\n";


            return min;
        }


        if (current > max)
        {
            std::cout << "\n\n\n";
            std::cout << name << " too high. Using maximum value.\n";
            std::cout << "\n\n\n";


            return max;
        }


        return current;
    }



    template<typename Object>
    bool isEmpty(const Object& obj)
    {
        return obj.isEmpty();
    }

	
}
