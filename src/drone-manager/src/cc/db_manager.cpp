#include "db_manager.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using std::string;

using namespace drone_manager;

MongoDB_Manager::MongoDB_Manager(const std::string &u) : uri(u)
{
    client = mongocxx::client(uri);
}

// Getters
