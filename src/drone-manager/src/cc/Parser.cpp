#include "Parser.hpp"

using namespace drone_manager;

std::vector<std::string> split(std::string str, const std::string &delimiter)
{
    std::vector<std::string> result;
    std::string aux = "";
    size_t i = 0;
    while ((i = str.find(delimiter)) != std::string::npos)
    {
        aux = str.substr(0, i);
        str.erase(0, i + delimiter.length());
        result.push_back(aux);
    }
    return result;
}