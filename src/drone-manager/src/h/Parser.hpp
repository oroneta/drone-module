#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <variant>

// TODO: Create my own parser or use opensource

namespace drone_manager
{
    class Parser
    {
    private:
    public:
        static std::vector<std::string> split(std::string str, const std::string &delimiter);
    };
}

#endif