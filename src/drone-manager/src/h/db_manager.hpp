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

// Before cox
namespace drone_manager
{
    const std::string uri_string = "mongodb://Oroneta_Admin:Oroneta_Password@oroneta.drone-module.drone-mongo:27017";
    const std::string db_name = "drone-module-db";
    class MongoDB_Manager
    {
    public:
        // funtions
        static mongocxx::client connect(const std::string &uri);
        static std::vector<std::pair<double, double>> getFlightPath(mongocxx::client &client, const std::string &dic, const std::string &db_name, const std::string &collect);

    private:
    };
}

#endif
