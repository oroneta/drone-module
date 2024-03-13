#include "AMDP_Server.hpp"
// TODO: Server that processes drone requests and responses

// constructor
AMDP_Server::AMDP_Server(const int socket_fd, mongocxx::client *client) : socket_with_client(socket_fd), mongo_client(client)
{
}

int AMDP_Server::get_socket_with_client() const {
    return socket_with_client;
}

void start() {
    
}