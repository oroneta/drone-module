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

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "drone.hpp"

// Port Numbers
#define MONGO_DB_PORT 27017
#define DRONE_API_PORT 60001
#define DRONE_MANAGER_PORT 60002
#define DRONE_AI_PORT 60003
/*********************************************************************************************************/

// Status codes
// 1xx
#define AMDP_SERVER_OVERLOAD 100

// 2xx
#define AMDP_SERVER_OK 200
#define AMDP_SERVER_OK_MODIFY 201

// 3xx
#define AMDP_SERVER_MOVED 300

// 4xx
#define AMDP_SERVER_SYNTAX_ERROR 400
#define AMDP_SERVER_AUNTHENTICATION_FAILED 401

// 5xx
#define AMDP_SERVER_COLLAPSED 500
#define AMDP_SERVER_CANNOT_RESPOND 501

/*********************************************************************************************************/
#define SOCKET_CREATION_FAILED -1
#define SOCKET_BIND_FAILED -2
#define SOCKET_LISTEN_FAILED -3
#define SOCKET_CLIENT_FAILED -4
#define SOCKET_READ_FAILED -5
#define SOCKET_SETOPT_FAILED -6
/*********************************************************************************************************/

#define WRONG_MESSAGE_TYPE -1
#define DRONE_CODE_NO_EXIST -2
/*********************************************************************************************************/
#define BUFFER_SIZE 1024

class AMDP_Server
{
private:
    int socket_with_client;
    // sockaddr_in client_address;
    std::shared_ptr<mongocxx::client> mongo_client;
    //TODO: implement--------------------------------------------
    void handle_wake_message(std::vector<std::string> &messages);

    void handle_push_message(std::vector<std::string> &messages);

    void handle_receive_message(std::vector<std::string> &messages);

    void handle_request_message(std::vector<std::string> &messages);

    void handle_response_message(std::vector<std::string> &messages);

    void handle_close_message(std::vector<std::string> &messages);

    void handle_end_message(std::vector<std::string> &messages);

    static float check_orientation(const drone_manager::Coordinate &coord);

    int amdp_protocol(char* buffer);
    //TODO: implement--------------------------------------------
public:
    // Constructors
    AMDP_Server(const int socket_fd, mongocxx::client *client);
    // Destructor
    ~AMDP_Server() {}

    // Getters
    int get_socket_with_client() const;
    // Setters
    void start();
    // Other methods
    int get_message_type(const std::string &msg) const;

    std::string get_message_string(const MessageType m) const; 

    void treat_message(std::vector<std::string> &msg) const; //TODO
};

enum MessageType
{
    WAKE = 1,
    PUSH = 2,
    RECEIVE = 3,
    REQUEST = 4,
    RESPONSE = 5,
    CLOSE = 6,
    END = 7
};

#endif