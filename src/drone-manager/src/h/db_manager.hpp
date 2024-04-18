#ifndef __DB_MANAGER_H__
#define __DB_MANAGER_H__

#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <string>

#define MONGO_DB_PORT 27017

// Before co
namespace drone_manager
{
    class MongoDB_Manager
    {
    public:
        MongoDB_Manager(const std::string &u);

        ~MongoDB_Manager() {}

        // Getters
        mongocxx::client &get_client() const;
        mongocxx::uri get_uri();
        mongocxx::database &get_database(std::string db_name) const;
        mongocxx::collection &get_collection(mongocxx::database &db, std::string coll_name) const;
        // other methods
        void change_uri(mongocxx::uri &u);

    private:
        mongocxx::uri uri;
        mongocxx::client client;
    };
}

#endif
