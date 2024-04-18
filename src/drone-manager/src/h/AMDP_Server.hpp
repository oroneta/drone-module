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

/*********************************************************************************************************/

/*********************************************************************************************************/
#define BUFFER_SIZE 1024

class AMDP_Server
{
private:
    int socket_with_client;
    // sockaddr_in client_address;
    std::shared_ptr<mongocxx::client> mongo_client;

    std::shared_ptr<mavsdk::Mavsdk> mavsdk;

    //std::mutex server_mutex;
    //TODO: implement--------------------------------------------
    /*void handle_wake_message(std::vector<std::string> &messages);

    void handle_push_message(std::vector<std::string> &messages);

    void handle_receive_message(std::vector<std::string> &messages);

    void handle_request_message(std::vector<std::string> &messages);

    void handle_response_message(std::vector<std::string> &messages);

    void handle_close_message(std::vector<std::string> &messages);

    void handle_end_message(std::vector<std::string> &messages);

    static float check_orientation(const drone_manager::Coordinate &coord);*/

    int amdp_protocol();

    //void show_drone_info() const;

    /**
     * @brief Function for getting routes from mongodb and prepare the mission for the drone
     * @param Info::Identification
     * @return Mission::MissionPlan
    */
    Mission::MissionPlan prepare_mission(Info::Identification id) const;

    /**
     * @brief Function for checking if drone is registered
     * @param Info::Identification
     * @return bool
    */
    bool check_drone_identification(Info::Identification id) const;
    //TODO: implement--------------------------------------------
public:
    // Constructors
    AMDP_Server(const int socket_fd, mongocxx::client *client, mavsdk::Mavsdk *mav);
    // Destructor
    ~AMDP_Server() {}

    // Getters
    int get_socket_with_client() const;
    // Setters
    
    // Other methods
    //int get_message_type(const std::string &msg) const;

    //std::string get_message_string(const MessageType m) const; 

    //void treat_message(std::vector<std::string> &msg) const; //TODO
    void start();

    /*void check_new_connection();*/
};

#endif