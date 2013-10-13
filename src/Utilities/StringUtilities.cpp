#include "StringUtilities.hpp"

void replaceString(std::string & str, std::string toReplace, std::string replaceWith)
{
    str.replace(str.find(toReplace), toReplace.length(), replaceWith);
}

