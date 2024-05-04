#include "db_manager.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using std::string;

using namespace drone_manager;

const std::string MongoDB_Manager::uri_string = "mongodb://Oroneta_Admin:Oroneta_Password@oroneta.drone-module.drone-mongo:27017";
const std::string MongoDB_Manager::db_name = "drone-module-db";


MongoDB_Manager::MongoDB_Manager(const std::string &u) : uri(u)
{
}


void MongoDB_Manager::connect() {
    try {
        client = mongocxx::client{mongocxx::uri{uri_string}};
        db = client[db_name];  
        std::cout << "Connected successfully to database: " << dbName << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Failed to connect to the database: " << e.what() << std::endl;
    }
}


static std::vector<std::pair<double, double>> getFlightPath(mongocxx::client& client, const std::string& dic) {

    mongocxx::database db = client["db_name"];  
    mongocxx::collection dronesCollection = db["drones"];

    auto doc = dronesCollection.find_one(bsoncxx::builder::stream::document{} << "dic" << droneId << bsoncxx::builder::stream::finalize);
    if (!doc) {
        std::cerr << "No drone found with dic: " << droneId << std::endl;
        return "";
    }

    auto view = doc->view();
    document::element flightElement = view["flight"];
    
    // if (flightElement.type() != type::k_array) {
    //     std::cerr << "Flight data is not in the expected format." << std::endl;
    //     return {};
    // }
   
    std::vector<std::pair<double, double>> flightPath;
    for (auto subArrayElement : flightElement.get_array().value) {
        auto iter = subArrayElement.get_array().value.begin();
        double latitude = (*iter++).get_double().value; 
        double longitude = (*iter).get_double().value;   

        flightPath.emplace_back(latitude, longitude);
    }
    return flightPath;
}


// Getters
