#include "AMDP_Server.hpp"
#include "Parser.hpp"
#include <sstream>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

// TODO: Server that processes drone requests and responses

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using drone_manager::Parser;
using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

// constructor
AMDP_Server::AMDP_Server(const int socket_fd, mongocxx::client *client, Mavsdk *mav) : socket_with_client(socket_fd), mongo_client(client), mavsdk(mav)
{
}

int AMDP_Server::get_socket_with_client() const
{
    return socket_with_client;
}

void AMDP_Server::start()
{
    
    int res = amdp_protocol();

    while (true) {}
}

int AMDP_Server::amdp_protocol()
{
    ConnectionResult res = mavsdk->add_udp_connection(atoi(getenv("MANAGER_PORT")), ForwardingOption::ForwardingOn); //getenv("MANAGER_PORT");
    if(res != ConnectionResult::Success) {
        std::cerr << "Connection errors: " << res << std::endl;
        return CONNECTION_FAILED;
    }

    auto system = mavsdk->first_autopilot(-1.0); //waiting for drone no timeout, negative value means no timeout
    if(!system) {
        std::cerr << "Timed out! No drone connection\n";
        return TIMEOUT_CONNECTION;
    }

    auto info = Info{system.value()};
    
    auto id = info.get_identification();
    if(id.first != Info::Result::Success) {
        std::cerr << "Error: cannot get drone identification\n";
        return IDENTIFICATION_ERROR;
    }

    if(check_drone_identification(id.second)) {
        std::cerr << "Error: drone no registered\n";
        return DRONE_NO_REGISTERED;
    }

    auto plan = prepare_mission(id.second);

    auto action = Action{system.value()};
    auto mission = Mission{system.value()};
    auto telemetry = Telemetry{system.value()};

    const auto set_rate_result = telemetry.set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success) {
        std::cerr << "Setting rate failed: " << set_rate_result << '\n';
        return 1;
    }

    telemetry.subscribe_position([](Telemetry::Position position) {
        std::cout << "Altitude: " << position.relative_altitude_m << " m\n";
        std::cout << "Absolute Altitude: " << position.absolute_altitude_m << " m\n";
        std::cout << "Latitude: " << position.latitude_deg << " deg\n";
        std::cout << "Longitude: " << position.longitude_deg << " deg\n";
    });

    telemetry.subscribe_heading([](Telemetry::Heading heading) {
        std::cout << "Heading: " << heading.heading_deg << " deg\n";
    });

    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready\n";
        sleep_for(std::chrono::seconds(3));
    }
    
    // Take off
    std::cout << "Taking off...\n";
    const Action::Result takeoff_result = action.takeoff();
    if (takeoff_result != Action::Result::Success) {
        std::cerr << "Takeoff failed: " << takeoff_result << '\n';
        return 1;
    }

    // Let it hover for a bit before landing again.
    sleep_for(seconds(10));

    std::cout << "Landing...\n";
    const Action::Result land_result = action.land();
    if (land_result != Action::Result::Success) {
        std::cerr << "Land failed: " << land_result << '\n';
        return 1;
    }

        // Check if vehicle is still in air
    while (telemetry.in_air()) {
        std::cout << "Vehicle is landing...\n";
        sleep_for(seconds(1));
    }
    std::cout << "Landed!\n";

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished...\n";

}

Mission::MissionPlan prepare_mission(Info::Identification id) {
    //! TODO: 
    
}

bool check_drone_identification(Info::Identification id) {
    //TODO: Future version. For now, return always true.
    return true;
}

/*
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
*/
