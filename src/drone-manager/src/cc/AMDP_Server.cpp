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

     while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready\n";
        sleep_for(seconds(1));
    }

    std::cout << "System ready\n";
    std::cout << "Creating and uploading mission\n";


    std::vector<Mission::MissionItem> mission_items = prepare_mission();

    std::cout << "Uploading mission...\n";
    Mission::MissionPlan mission_plan{};
    mission_plan.mission_items = mission_items;
    const Mission::Result upload_result = mission.upload_mission(mission_plan);

    if (upload_result != Mission::Result::Success) {
        std::cerr << "Mission upload failed: " << upload_result << ", exiting.\n";
        return 1;
    }

    std::cout << "Arming...\n";
    const Action::Result arm_result = action.arm();
    if (arm_result != Action::Result::Success) {
        std::cerr << "Arming failed: " << arm_result << '\n';
        return 1;
    }
    std::cout << "Armed.\n";

    std::atomic<bool> want_to_pause{false};
    // Before starting the mission, we want to be sure to subscribe to the mission progress.
    mission.subscribe_mission_progress([&want_to_pause](Mission::MissionProgress mission_progress) {
        std::cout << "Mission status update: " << mission_progress.current << " / "
                  << mission_progress.total << '\n';

        if (mission_progress.current >= 2) {
            // We can only set a flag here. If we do more request inside the callback,
            // we risk blocking the system.
            want_to_pause = true;
        }
    });

    Mission::Result start_mission_result = mission.start_mission();
    if (start_mission_result != Mission::Result::Success) {
        std::cerr << "Starting mission failed: " << start_mission_result << '\n';
        return 1;
    }

    while (!want_to_pause) {
        sleep_for(seconds(1));
    }

    sleep_for(seconds(20));

    while (!mission.is_mission_finished().second) {
        sleep_for(seconds(1));
    }

    // We are done, and can do RTL to go home.
    std::cout << "Commanding RTL...\n";
    const Action::Result rtl_result = action.return_to_launch();
    if (rtl_result != Action::Result::Success) {
        std::cout << "Failed to command RTL: " << rtl_result << '\n';
        return 1;
    }
    std::cout << "Commanded RTL.\n";

    // We need to wait a bit, otherwise the armed state might not be correct yet.
    sleep_for(seconds(2));

    while (telemetry.armed()) {
        // Wait until we're done.
        sleep_for(seconds(1));
    }
    std::cout << "Disarmed, exiting.\n";

    return 0;
}

int AMDP_Server::amdp_protocol()
{

    return 0;
}

std::vector<mavsdk::Mission::MissionItem> AMDP_Server::prepare_mission() const
{
    std::vector<mavsdk::Mission::MissionItem> mission_items;

     mission_items.push_back(make_mission_item2(
        39.4808152,
        -0.3401091,
        2.0f,
        5.0f,
        true,
        0.0f,
        0.0f,
        Mission::MissionItem::CameraAction::None));

    mission_items.push_back(make_mission_item2(
        39.4400359,
        -0.3222084,
        2.0f,
        15.0f,
        true,
        0.0f,
        0.0f,
        Mission::MissionItem::CameraAction::None));

    mission_items.push_back(make_mission_item2(
        39.4336000,
        -0.3123000,
        2.0f,
        15.0f,
        true,
        0.0f,
        0.0f,
        Mission::MissionItem::CameraAction::TakePhoto));

    mission_items.push_back(make_mission_item2(
        39.4299000,
        -0.3152000,
        2.0f,
        15.0f,
        true,
        0.0f,
        0.0f,
        Mission::MissionItem::CameraAction::StartVideo));

    mission_items.push_back(make_mission_item2(
        39.4359000,
        -0.3301000,
        2.0f,
        15.0f,
        true,
        0.0f,
        0.0f,
        Mission::MissionItem::CameraAction::StopVideo));

    mission_items.push_back(make_mission_item2(
        39.4400359,
        -0.3222084,
        2.0f,
        15.0f,
        true,
        0.0f,
        0.0f,
        Mission::MissionItem::CameraAction::StartPhotoInterval));

    return mission_items;

}

mavsdk::Mission::MissionItem AMDP_Server::make_mission_item2(
    double latitude_deg,
    double longitude_deg,
    float relative_altitude_m,
    float speed_m_s,
    bool is_fly_through,
    float gimbal_pitch_deg,
    float gimbal_yaw_deg,
    mavsdk::Mission::MissionItem::CameraAction camera_action) const
{
    mavsdk::Mission::MissionItem new_item{};
    new_item.latitude_deg = latitude_deg;
    new_item.longitude_deg = longitude_deg;
    new_item.relative_altitude_m = relative_altitude_m;
    new_item.speed_m_s = speed_m_s;
    new_item.is_fly_through = is_fly_through;
    new_item.gimbal_pitch_deg = gimbal_pitch_deg;
    new_item.gimbal_yaw_deg = gimbal_yaw_deg;
    new_item.camera_action = camera_action;
    return new_item;
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
