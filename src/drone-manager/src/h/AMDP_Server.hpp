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
#define IDENTIFICATION_ERROR -3
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
     * @brief Function for getting routes from mongodb and prepare the mission for the drone
     * @param Info::Identification
     * @return Mission::MissionPlan
     */
    std::vector<mavsdk::Mission::MissionItem> prepare_mission(mavsdk::Telemetry::Position &pos) const;

    mavsdk::Mission::MissionPlan takeOff_land_mission(mavsdk::Telemetry::Position pos) const;

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

    // Other methods
    // int get_message_type(const std::string &msg) const;

    // std::string get_message_string(const MessageType m) const;

    // void treat_message(std::vector<std::string> &msg) const; //TODO
    int start();

    void close_port_connections();

    std::shared_ptr<mongocxx::client> get_client() const;

    /*void check_new_connection();*/
};

#endif