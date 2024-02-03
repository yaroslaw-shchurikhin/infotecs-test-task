#include "user_interface.hpp"

#include <string>
#include <limits>
#include <iostream>
#include <algorithm>

#include "../supply/support_funcs.hpp"

namespace ui
{

UserInterface::UserInterface(std::istream& input) :
    in_(input)
    {}

void UserInterface::start(detail::Buffer<std::string>& buf)
{
    constexpr uint8_t MAX_STRING_LENGTH = 64u;
    for(;;)
    {
        std::string input_str;
        std::getline(in_, input_str);

        if(input_str.size() <= MAX_STRING_LENGTH && input_str.size() > 0 && supply::isDigitString(input_str))
        {
            std::sort(input_str.begin(), input_str.end(), [](char x, char y){ return x > y;});
            supply::replaceEven(input_str);
            buf.push_back(input_str);
        }
        else
        {
            std::cerr << "INVALID INPUT\n";
        }
    }

}
} // namespace ui
