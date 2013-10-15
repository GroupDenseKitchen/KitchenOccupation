#include "StringUtilities.hpp"

void replaceString(std::string & str, std::string toReplace, std::string replaceWith)
{
    int pos = str.find(toReplace);
    if(pos >= 0)
        str.replace(str.find(toReplace), toReplace.length(), replaceWith);
}

