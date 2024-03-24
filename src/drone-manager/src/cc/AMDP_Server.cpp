#include "AMDP_Server.hpp"
#include "Parser.hpp"
#include <sstream>
#include <vector>
#include <memory>
#include <mavsdk/mavsdk.h>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
// TODO: Server that processes drone requests and responses

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using drone_manager::Parser;

// constructor
AMDP_Server::AMDP_Server(const int socket_fd, mongocxx::client *client) : socket_with_client(socket_fd), mongo_client(client)
{
}

int AMDP_Server::get_socket_with_client() const
{
    return socket_with_client;
}

void AMDP_Server::start()
{
    char *buffer = new char[BUFFER_SIZE]; // TODO: temporal buffer size
    memset(buffer, '\0', BUFFER_SIZE);

    delete[] buffer;
}

int AMDP_Server::amdp_protocol(char *buffer)
{
    int bytes_read = recv(socket_with_client, buffer, BUFFER_SIZE, 0);

    if (bytes_read == -1)
    {
        std::cerr << "Read from socket failed failed" << std::endl;
        exit(SOCKET_READ_FAILED);
    }
    std::string message(buffer);
    std::vector<std::string> lines = Parser::split(message, "\r\n");

    if (get_message_type(lines[1]) != WAKE)
    {
        // Check that it starts with a wake message
        std::cout << "Warning!: It is not a start message!" << std::endl;
        memset(buffer, 0, BUFFER_SIZE);
        strcpy(buffer, "Closing connection, it is not a wake message!");
        send(socket_with_client, buffer, strlen(buffer), 0);
        close(socket_with_client);
        return WRONG_MESSAGE_TYPE;
    }

    std::string drone_code = lines[2];
    std::string drone_auth_code = lines[3];
    // get the database and drones table
    mongocxx::database drone_db = (*mongo_client)[getenv("MONGO_DBNAME")];
    mongocxx::collection drone_collection = drone_db["drones"];

    // Prepare document for query
    bsoncxx::document::view query = make_document(
        kvp("dic", drone_code),
        kvp("auth_code", drone_auth_code));

    // query for the specific drone information in the database
    auto drone = drone_collection.find_one(query);
    if (!drone.has_value())
    { // Drone not found in the mongo database
        std::cout << "Error: No drone found in the mongo database" << std::endl;
        std::cout << "drone_code: " << drone_code << std::endl;
        memset(buffer, 0, BUFFER_SIZE);
        strcpy(buffer, "Closing connection, the drone code specified does not exist!");
        send(socket_with_client, buffer, strlen(buffer), 0);
        close(socket_with_client);
        return DRONE_CODE_NO_EXIST;
    }
}

int AMDP_Server::get_message_type(const std::string &mg) const
{
    if (mg == "WAKE")
        return MessageType::WAKE;
    else if (mg == "PUSH")
        return MessageType::PUSH;
    else if (mg == "RECEIVE")
        return MessageType::RECEIVE;
    else if (mg == "REQUEST")
        return MessageType::REQUEST;
    else if (mg == "RESPONSE")
        return MessageType::RESPONSE;
    else if (mg == "CLOSE")
        return MessageType::CLOSE;
    else if (mg == "END")
        return MessageType::END;
    else
        return -1;
}

std::string AMDP_Server::get_message_string(const MessageType m) const
{
    std::string message = "";
    switch (m)
    {
    case WAKE:
        message = "WAKE";
        break;
    case PUSH:
        message = "PUSH";
        break;
    case RECEIVE:
        message = "RECEIVE";
        break;
    case REQUEST:
        message = "REQUEST";
        break;
    case RESPONSE:
        message = "RESPONSE";
        break;
    case CLOSE:
        message = "CLOSE";
        break;
    case END:
        message = "END";
        break;
    default:
        break;
    }

    return message;
}