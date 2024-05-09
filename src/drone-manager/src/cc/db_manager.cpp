#include "db_manager.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using std::string;

using namespace drone_manager;

std::shared_ptr<mongocxx::client> MongoDB_Manager::connect(const std::string &uri)
{
    std::shared_ptr<mongocxx::client> client;
    try
    {
        client = std::shared_ptr<mongocxx::client>(new mongocxx::client{mongocxx::uri{uri}});
        std::cout << "Connected successfully to database: " << db_name << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to connect to the database: " << e.what() << std::endl;
    }
    return client;
}

std::vector<std::pair<double, double>> MongoDB_Manager::getFlightPath(mongocxx::client &client, const std::string &dic, const std::string &db_name, const std::string &collect)
{

    mongocxx::database db = client[db_name];
    mongocxx::collection dronesCollection = db[collect];

    auto doc = dronesCollection.find_one(make_document(kvp("dic", dic)));
    if (!doc)
    {
        std::cerr << "No drone found with dic: " << dic << std::endl;
        return {};
    }

    // auto view = doc.value()["dic"];
    auto dicc = doc.value()["dic"].get_string().value;
    auto flight = doc.value()["flight"].get_array();
    // auto flightString = flight.value.to_string();

    // if (flightElement.type() != type::k_array) {
    //     std::cerr << "Flight data is not in the expected format." << std::endl;
    //     return {};
    // }
    std::cout << "Flight attribute: " << std::endl;

    std::vector<std::pair<double, double>> flightPath;
    auto array = flight.value;
    if (array.length() == 0)
    {
        return {};
    }
    for (auto subArrayElement : flight.value)
    {
        //if the value in the db is integer, then this will throw an exception
        double latitude = subArrayElement[0].get_value().get_double();
        double longitude = subArrayElement[1].get_value().get_double();
        std::cout << latitude << " " << longitude << std::endl;
        flightPath.emplace_back(latitude, longitude);
    }
    return flightPath;
}

void MongoDB_Manager::updateDronePosition(mongocxx::client &client,const std::string &dic, const std::string &db_name, const std::string &collect, double latitude, double longitude) {
    mongocxx::database db = client[db_name];
    mongocxx::collection dronesCollection = db[collect];

    auto filter = make_document(kvp("dic", dic));

    auto update = make_document(kvp("$set", make_document(kvp("metadata.gps", make_array(latitude, longitude)))));

    try {
        auto result = dronesCollection.update_one(filter.view(), update.view());
        //
        if (result && result->modified_count() > 0) {
            std::cout << "Successfully updated drone position " << dic << std::endl;
        } else {
            std::cout << "No drone updated  " << dic << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error updating drone position: " << e.what() << std::endl;
    }
}

void MongoDB_Manager::updateDroneBattery(mongocxx::client &client, const std::string &db_name, const std::string &collect, const std::string &dic, double battery) {
    mongocxx::database db = client[db_name];
    mongocxx::collection dronesCollection = db[collect];

    auto filter = make_document(kvp("dic", dic));
    auto update = make_document(kvp("$set", make_document(kvp("metadata.battery", battery))));

    try {
        auto result = dronesCollection.update_one(filter.view(), update.view());
        if (result && result->modified_count() > 0) {
            std::cout << "Successfully updated battery  " << dic << std::endl;
        } else {
            std::cout << "No updated battery " << dic << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error updating battery level: " << e.what() << std::endl;
    }
}

void MongoDB_Manager::updateDroneSpeed(mongocxx::client &client, const std::string &db_name, const std::string &collect, const std::string &dic, double speed) {
    mongocxx::database db = client[db_name];
    mongocxx::collection dronesCollection = db[collect];

    auto filter = make_document(kvp("dic", dic));
    auto update = make_document(kvp("$set", make_document(kvp("metadata.speed", speed))));

    try {
        auto result = dronesCollection.update_one(filter.view(), update.view());
        if (result && result->modified_count() > 0) {
            std::cout << "Successfully updated speed" << dic << std::endl;
        } else {
            std::cout << "No speed updated" << dic << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error updating speed: " << e.what() << std::endl;
    }
}

// Getters
