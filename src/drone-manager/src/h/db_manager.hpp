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
        /**
         * @brief Establishes a connection to the MongoDB server using the provided URI.
         * 
         * @param uri MongoDB connection URI string.
         * @return std::shared_ptr<mongocxx::client> Pointer to the MongoDB client.
         */
        static std::shared_ptr<mongocxx::client> connect(const std::string &uri);

        /**
         * @brief Retrieves the flight path coordinates from the specified MongoDB collection.
         * 
         * @param client MongoDB client connection.
         * @param db_name Name of the MongoDB database.
         * @param collect Name of the MongoDB collection containing flight data.
         * @return std::vector<std::pair<double, double>> List of latitude and longitude coordinates.
         */
        static std::vector<std::pair<double, double>> getFlightPath(mongocxx::client &client, const std::string &dic, const std::string &db_name, const std::string &collect);

        /**
         * @brief Updates the drone's position in the MongoDB collection.
         * 
         * @param client MongoDB client connection.
         * @param db_name Name of the MongoDB database.
         * @param collect Name of the MongoDB collection for drone data.
         * @param latitude Latitude coordinate of the drone.
         * @param longitude Longitude coordinate of the drone.
         */
        static void updateDronePosition(mongocxx::client &client,const std::string &dic, const std::string &db_name, const std::string &collect, double latitude, double longitude);

        /**
         * @brief Updates the drone's battery level in the MongoDB collection.
         * 
         * @param client MongoDB client connection.
         * @param db_name Name of the MongoDB database.
         * @param collect Name of the MongoDB collection for drone data.
         * @param dic Drone identifier code.
         * @param battery Current battery level of the drone.
         */
        static void updateDroneBattery(mongocxx::client &client, const std::string &db_name, const std::string &collect, const std::string &dic, double battery);

        /**
         * @brief Updates the drone's speed in the MongoDB collection.
         * 
         * @param client MongoDB client connection.
         * @param db_name Name of the MongoDB database.
         * @param collect Name of the MongoDB collection for drone data.
         * @param dic Drone identifier code.
         * @param speed Current speed of the drone.
         */
        static void updateDroneSpeed(mongocxx::client &client, const std::string &db_name, const std::string &collect, const std::string &dic, double speed);
    private:
    };
}

#endif
