#include "drone.hpp"
#include "route.hpp"
#include "db_manager.hpp"
#include "AMDP_Server.hpp"

#include <cstdint>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <future>
#include <functional>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

// mongocxx
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

using namespace mavsdk;



int main(int argc, char **argv)
{

    mongocxx::instance instance{}; // Only once, don't invoke again. For more information, see mongocxx driver tutorial page
    // Neither client and pool are safely copied after fork, so we need to create new clients and pool after fork. More information mongocxx documentation
    // In addition, all the mongocxx objects are not safely copied after fork
    //mongocxx::uri uri("mongodb://localhost:27017"); // TODO: Hardcoded
    string server_ip = "mongodb://oroneta.drone-module.drone-mongo:27017";
    //server_ip = server_ip + getenv("MONGO_SERVR") + ":" + getenv("MONGO_DBPORT");
    mongocxx::client *client;
    try {
        client = new mongocxx::client{mongocxx::uri{server_ip}};
    } catch (const std::exception &e) {
        cout << e.what() << endl;
    }


    Mavsdk *mavsdk = new Mavsdk{Mavsdk::Configuration{Mavsdk::ComponentType::GroundStation}};

    AMDP_Server server(-1, client, mavsdk);  

    server.start();
      
    return 0;
}
