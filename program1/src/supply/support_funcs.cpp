#include "support_funcs.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>

namespace supply
{
    bool isDigitString(const std::string& str)
    {
        for(char i : str)
        {
            if(!isdigit(i))
            {
               return false;
            }
        }

        return true;
    }

    void replaceEven(std::string& str)
    {
        constexpr char FILLER[] = "KB";

        for(size_t i = 0; i < str.length(); ++i)
        {
            if((str[i] - '0') % 2 == 0)
            {

                str.replace(i, 1, FILLER);
                i += std::string(FILLER).length() - 1;
            }
        }
    }
} // namespace supply
