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

int AMDP_Server::start()
{
    //AMDP DEFAULT PORT 60002
    /*ConnectionResult res = mavsdk->add_udp_connection(DRONE_MANAGER_PORT, ForwardingOption::ForwardingOff); // atoi(getenv("MANAGER_PORT"))
    if (res != ConnectionResult::Success)
    4
        std::cerr << "Connection errors: " << res << std::endl;
        return CONNECTION_FAILED;
    }*/

    //USING MAVLINK DEFAULT PORT
    ConnectionResult res = mavsdk->add_udp_connection(14540, ForwardingOption::ForwardingOff); //
    if (res != ConnectionResult::Success)
    {
        std::cerr << "Connection errors: " << res << std::endl;
        return CONNECTION_FAILED;
    }

    auto system = mavsdk->first_autopilot(-1.0); // waiting for drone, negative value means no timeout
    if (!system)
    {
        std::cerr << "Timed out! No drone connection\n";
        return TIMEOUT_CONNECTION;
    }

    auto info = Info{system.value()};

    auto id = info.get_identification();
    if (id.first != Info::Result::Success)
    {
        std::cerr << "Error: cannot get drone identification\n";
        return IDENTIFICATION_ERROR;
    }

    if (!check_drone_identification(id.second))
    {
        std::cerr << "Error: drone no registered\n";
        std::cerr << id.second.hardware_uid << std::endl;
        return DRONE_NO_REGISTERED;
    }

    std::cout << "Drone ID: " << id.second.hardware_uid << std::endl;

    // auto plan = prepare_mission(id.second); // get the mission path from database base on the drone ID

    auto action = Action{system.value()};
    auto mission = Mission{system.value()};
    auto telemetry = Telemetry{system.value()};

    const auto set_rate_result = telemetry.set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_result << '\n';
        return 1;
    }

    telemetry.subscribe_position([](Telemetry::Position position)
                                 {
        std::cout << "Altitude: " << position.relative_altitude_m << " m\n";
        //std::cout << "Absolute Altitude: " << position.absolute_altitude_m << " m\n";
        std::cout << "Latitude: " << position.latitude_deg << " deg\n";
        std::cout << "Longitude: " << position.longitude_deg << " deg\n"; });

    telemetry.subscribe_heading([](Telemetry::Heading heading)
                                { std::cout << "Heading: " << heading.heading_deg << " deg\n"; });

    while (!telemetry.health_all_ok())
    {
        std::cout << "Waiting for system to be ready\n";
        sleep_for(std::chrono::seconds(2));
    }

    std::cout << "Arming drone\n";
    const Action::Result actionRes = action.arm();
    if (actionRes != Action::Result::Success)
    {
        std::cerr << "Arming drone failed" << std::endl;
        return DRONE_ARM_ERROR;
    }
    std::cout << "Drone armed successfully\n";

    std::atomic<bool> want_to_pause{false};

    mission.subscribe_mission_progress([&want_to_pause](Mission::MissionProgress mission_progress)
                                       { std::cout << "Mission status update: " << mission_progress.current << " / "
                                                   << mission_progress.total << '\n'; });

    action.set_takeoff_altitude(2.0f);
    // Take off
    std::cout << "Takeoff order!\n";
    const Action::Result takeoff_result = action.takeoff();
    if (takeoff_result != Action::Result::Success)
    {
        std::cerr << "Takeoff failed: " << takeoff_result << '\n';
        return DRONE_TAKEOFF_ERROR;
    }

    auto plan = takeOff_land_mission(telemetry.position());
    Mission::Result missionRes = mission.upload_mission(plan);
    if (missionRes != Mission::Result::Success)
    {
        std::cerr << "Failed to upload mission" << std::endl;
        return DRONE_MISSION_UPLOAD_ERROR;
    }

    if (mission.start_mission() != Mission::Result::Success)
    {
        std::cerr << "Failed to start mission" << std::endl;
        return DRONE_MISSION_START_FAILED;
    }

    do
    {

        // Let it hover for a bit before landing again.
        sleep_for(seconds(20));

    } while (!mission.is_mission_finished().second);

    std::cout << "Landing...\n";
    const Action::Result land_result = action.land();
    if (land_result != Action::Result::Success)
    {
        std::cerr << "Land failed: " << land_result << '\n';
        return DRONE_LANDING_ERROR;
    }

    // Check if vehicle is still in air
    std::cout << "landing...";
    while (telemetry.in_air())
    {
        std::cout << ".";
        sleep_for(seconds(1));
    }
    std::cout << "\n";
    std::cout << "Landing completed!\n";

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(5));

    std::cout << "Mission finished....\n";
    while (telemetry.armed())
    {
        sleep_for(seconds(2));
    }

    return 0;
}

int AMDP_Server::amdp_protocol()
{

    return 0;
}

mavsdk::Mission::MissionPlan AMDP_Server::prepare_mission(mavsdk::Info::Identification id) const
{
    //! TODO:
}

mavsdk::Mission::MissionPlan AMDP_Server::takeOff_land_mission(mavsdk::Telemetry::Position pos) const
{
    Mission::MissionPlan plan;

    Mission::MissionItem item1;
    item1.relative_altitude_m = 2.0f;
    item1.latitude_deg = pos.latitude_deg + 3.0;
    item1.longitude_deg = pos.longitude_deg + 3.0;
    plan.mission_items.push_back(item1);

    return plan;
}

bool AMDP_Server::check_drone_identification(mavsdk::Info::Identification id) const
{
    // TODO: Future version. For now, return always true.
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
