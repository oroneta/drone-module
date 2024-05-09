#ifndef __AMDP_SERVER_H__
#define __AMDP_SERVER_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <chrono>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mavsdk/mavsdk.h>
#include <future>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/info/info.h>

#include "drone.hpp"

// Port Numbers
#define MONGO_DB_PORT 27017
#define DRONE_API_PORT 60001
#define DRONE_MANAGER_PORT 60002
#define DRONE_AI_PORT 60003
/*********************************************************************************************************/

/*********************************************************************************************************/
#define CONNECTION_FAILED -1
#define TIMEOUT_CONNECTION -2
#define IDENTIFICATION_ERROR -3 ///< The One Ring remains unidentified (Error: Identification Failed).
#define DRONE_NO_REGISTERED -4
#define DRONE_LANDING_ERROR -5
#define DRONE_TAKEOFF_ERROR -6
#define DRONE_ARM_ERROR -7
#define DRONE_MISSION_UPLOAD_ERROR -8
#define DRONE_MISSION_START_FAILED -9
#define NO_MISSION_FOR_DRONE -10
#define MISSION_APLOAD_FAILED -11
/*********************************************************************************************************/

/*********************************************************************************************************/
#define BUFFER_SIZE 1024

/**
 * @class AMDP_Server
 * @brief Class responsible for managing communication with drones and MongoDB across the Middle-earth.
 */

class AMDP_Server
{
private:
    // sockaddr_in client_address;
    std::shared_ptr<mongocxx::client> mongo_client;

    std::shared_ptr<mavsdk::Mavsdk> mavsdk;

    // std::mutex server_mutex;
    // TODO: implement--------------------------------------------
    int amdp_protocol();

    /**
     * @brief In Esgaroth, prepares a mission plan for the drone based on routes fetched from MongoDB.
     * @param pos Current telemetry position of the drone.
     * @return std::vector<mavsdk::Mission::MissionItem> The prepared mission items.
     */
    std::vector<mavsdk::Mission::MissionItem> prepare_mission(mavsdk::Telemetry::Position &pos) const;

    /**
     * @brief Generates a takeoff and landing mission plan based on given telemetry position (Set coordinates for takeoff and landing).
     * @param pos Current telemetry position of the drone (Telemetry position: Ready to launch).
     * @return mavsdk::Mission::MissionPlan The generated mission plan (Mission plan: Locked and loaded).
     * Deprecated
     mavsdk::Mission::MissionPlan takeOff_land_mission(mavsdk::Telemetry::Position pos) const;**/

    /**

    * @brief Creates a new mission item for the drone's quest using the power of Middle-earth (and specified parameters).
    * 
    * This method crafts a mission item with coordinates and actions suitable for the drone's journey through Middle-earth.
    *
    * @param latitude_deg Latitude coordinate in degrees.
    * @param longitude_deg Longitude coordinate in degrees, navigating through the Mines of Moria.
    * @param relative_altitude_m Relative altitude above ground, in meters, soaring over the Misty Mountains.
    * @param speed_m_s Speed of the drone in meters per second, faster than the Eagles of Manwë.
    * @param is_fly_through Flag indicating if the drone should fly through the waypoints, akin to crossing the Paths of the Dead.
    * @param gimbal_pitch_deg Pitch angle of the gimbal in degrees, aiming to capture the beauty of Rivendell.
    * @param gimbal_yaw_deg Yaw angle of the gimbal in degrees, adjusting for different perspectives in the land of Gondor.
    * @param camera_action Camera action to perform during the mission, recording epic scenes worthy of the Red Book of Westmarch.
    * 
    * @return mavsdk::Mission::MissionItem The mission item ready to be part of the drone's epic quest.
    */
    mavsdk::Mission::MissionItem make_mission_item2(
        double latitude_deg,
        double longitude_deg,
        float relative_altitude_m,
        float speed_m_s,
        bool is_fly_through,
        float gimbal_pitch_deg,
        float gimbal_yaw_deg,
        mavsdk::Mission::MissionItem::CameraAction camera_action) const;

    /**
    * @brief Function for checking if drone is registered
    * @param Info::Identification
    * @return bool
    */
    bool check_drone_identification(mavsdk::Info::Identification id) const;

    // TODO: implement--------------------------------------------
public:
    // Constructors
    AMDP_Server(std::shared_ptr<mongocxx::client> client, mavsdk::Mavsdk *mav);
    // Destructor
    ~AMDP_Server() {}

    /**
    * @brief Embarks on an epic journey to command and guide the drone through Middle-earth.
    * 
    * This function represents the initiation of a grand quest, where the AMDP_Server sets forth to command and guide the drone across the lands of Middle-earth. The journey begins with preparations and validations akin to seeking fellowship and identifying trusted allies.
    * 
    * @return int Returns 0 upon successful completion of the mission, signaling a triumphant return home.
    */
    int start();

    std::shared_ptr<mongocxx::client> get_client() const;


    // Other methods
    // int get_message_type(const std::string &msg) const;

    // std::string get_message_string(const MessageType m) const;

    // void treat_message(std::vector<std::string> &msg) const; //TODO

    /*void close_port_connections();*/


    /*void check_new_connection();*/
};

#endif