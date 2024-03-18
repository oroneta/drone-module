#include "AMDP_Server.hpp"
#include <sstream>
#include <vector>
// TODO: Server that processes drone requests and responses

// Utility functions
std::vector<std::string> split(std::string str, const std::string &delimiter)
{
    std::vector<std::string> result;
    std::string aux = "";
    size_t i = 0;
    while ((i = str.find(delimiter)) != std::string::npos)
    {
        aux = str.substr(0, i);
        str.erase(0, i + delimiter.length());
        result.push_back(aux);
    }
    return result;
}

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

    int bytes_read = recv(socket_with_client, buffer, BUFFER_SIZE, 0);

    if (bytes_read == -1)
    {
        std::cerr << "Read from socket failed failed" << std::endl;
        exit(SOCKET_READ_FAILED);
    }
    std::string message(buffer);
    std::vector<std::string> lines = split(message, "\r\n");

    switch (get_message_type(lines[1]))
    {
    case MessageType::WAKE:
        /* code */
        break;

    case MessageType::PUSH:
        break;

    case MessageType::RECEIVE:
        break;

    case MessageType::REQUEST:
        break;

    case MessageType::RESPONSE:
        break;

    case MessageType::CLOSE:
        break;

    case MessageType::END:
        break;
    default:
        break;
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