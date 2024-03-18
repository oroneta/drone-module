#include "drone.hpp"
#include "route.hpp"
#include "db_manager.hpp"
#include "AMDP_Server.hpp"

#include <cstdint>
#include <iostream>
#include <vector>
#include <unistd.h>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>

#include <json/json.h>

// mongocxx
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

const char *server_ip = "127.0.0.1"; // TODO: Harcoded

int main(int argc, char **argv)
{

    mongocxx::instance instance{}; // Only once, don't invoke again. For more information, see mongocxx driver tutorial page
    // Neither client and pool are safely copied after fork, so we need to create new clients and pool after fork. More information mongocxx documentation
    // In addition, all the mongocxx objects are not safely copied after fork
    mongocxx::uri uri("mongodb://localhost:27017"); // TODO: Hardcoded
    mongocxx::client client(uri);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        cerr << "Failed to create socket!" << endl;
        return SOCKET_CREATION_FAILED;
    }

    cout << "Socket created!" << endl;
    int opt = 1;

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SOCK_NONBLOCK, &opt, sizeof(opt)))
    {
        return SOCKET_SETOPT_FAILED;
    }
    // Setting IPv4 address and port
    int port = DRONE_MANAGER_PORT;
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    // IP address
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr); // TODO: Hardcoded
    // Port
    server_addr.sin_port = htons(port);

    // Binding socket
    int res = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res == -1)
    {
        cerr << "Socket failed to bind!" << endl;
        return SOCKET_BIND_FAILED;
    }

    cout << "Socket bound successfully!" << endl;

    // Socket listening
    res = listen(server_socket, 100); // Although we are doing a concurrent server via processes, máx 100 connections in wait
    if (res == -1)
    {
        cerr << "Socket failed to listen" << endl;
        return SOCKET_LISTEN_FAILED;
    }

    cout << "Socket listening...." << endl;

    struct sockaddr_in client_addr;
    socklen_t client_length = 0;
    int client_socket = 0;
    while (true) // TODO: change infinite loop
    {
        client_length = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client, &client_length);

        pid_t pid = fork();
        if (pid == 0) // Is child process
        {
            if (client_socket == -1)
            {
                cerr << "Connection with client failed" << endl;
                close(server_socket);
                close(client_socket);
                return SOCKET_CLIENT_FAILED;
            }

            mongocxx::client *c = new mongocxx::client(uri);
            AMDP_Server server(client_socket, c);

            server.start();
        }
        else if (pid == -1) // process creating failed
        {
            cerr << "Failed to fork" << endl;
        }
        else
        { // Parent process

            if (client_socket != -1)
            {
                char *remote = (char *)calloc(INET_ADDRSTRLEN, sizeof(char));
                cout << "Client connection process PID: " << pid << endl;
                cout << "Client IP: " << inet_ntop(AF_INET, &client_addr.sin_addr, remote, INET_ADDRSTRLEN) << " ";
                cout << "Client port: " << ntohs(client_addr.sin_port) << endl;
                cout << endl;
                close(client_socket);
            }
        }
    }
    return 0;
}
