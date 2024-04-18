#include <iostream>
#include <AMDP_Server.hpp>
#include <future>
#include <iostream>
#include <thread>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/param_server/param_server.h>
#include <mavsdk/plugins/param/param.h>
#include <mavsdk/plugins/telemetry_server/telemetry_server.h>
#include <mavsdk/plugins/action_server/action_server.h>
#include <mavsdk/plugins/mission_raw_server/mission_raw_server.h>
#include <mavsdk/plugins/mission/mission.h>

using namespace mavsdk;

using std::cout;
using std::endl;

using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

Mission::MissionItem make_mission_item(
    double latitude_deg,
    double longitude_deg,
    float relative_altitude_m,
    float speed_m_s,
    bool is_fly_through,
    float gimbal_pitch_deg,
    float gimbal_yaw_deg,
    Mission::MissionItem::CameraAction camera_action)
{
    Mission::MissionItem new_item{};
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

int mai(int argc, char **argv)
{

    mavsdk::Mavsdk mavsdkTester{
        mavsdk::Mavsdk::Configuration{mavsdk::Mavsdk::ComponentType::Autopilot}};

    auto result = mavsdkTester.add_any_connection("udp://127.0.0.1:14540");
    if (result == mavsdk::ConnectionResult::Success)
    {
        std::cout << "Connected autopilot server side!" << std::endl;
    }

    auto server_component = mavsdkTester.server_component();

    // Create server plugins
    auto paramServer = mavsdk::ParamServer{server_component};
    auto telemServer = mavsdk::TelemetryServer{server_component};
    auto actionServer = mavsdk::ActionServer{server_component};

    // These are needed for MAVSDK at the moment
    paramServer.provide_param_int("CAL_ACC0_ID", 1);
    paramServer.provide_param_int("CAL_GYRO0_ID", 1);
    paramServer.provide_param_int("CAL_MAG0_ID", 1);
    paramServer.provide_param_int("SYS_HITL", 0);
    paramServer.provide_param_int("MIS_TAKEOFF_ALT", 0);
    // Add a custom param
    paramServer.provide_param_int("my_param", 1);

    // Allow the vehicle to change modes, takeoff and arm
    actionServer.set_allowable_flight_modes({true, true, true});
    actionServer.set_allow_takeoff(true);
    actionServer.set_armable(true, true);

    // Create vehicle telemetry info
    TelemetryServer::Position position{55.953251, -3.188267, 0, 0};
    TelemetryServer::PositionVelocityNed positionVelocityNed{{0, 0, 0}, {0, 0, 0}};
    TelemetryServer::VelocityNed velocity{};
    TelemetryServer::Heading heading{60};
    TelemetryServer::RawGps rawGps{
        0, 55.953251, -3.188267, 0, NAN, NAN, 0, NAN, 0, 0, 0, 0, 0, 0};
    TelemetryServer::GpsInfo gpsInfo{11, TelemetryServer::FixType::Fix3D};

    // Publish home already, so that it is available.
    telemServer.publish_home(position);

    // Create a mission raw server
    // This will allow us to receive missions from a GCS
    auto missionRawServer = mavsdk::MissionRawServer{server_component};
    std::cout << "MissionRawServer created" << std::endl;

    auto mission_prom = std::promise<MissionRawServer::MissionPlan>{};
    MissionRawServer::IncomingMissionHandle handle =
        missionRawServer.subscribe_incoming_mission(
            [&mission_prom, &missionRawServer, &handle](
                MissionRawServer::Result res, MissionRawServer::MissionPlan plan)
            {
                std::cout << "Received Uploaded Mission!" << std::endl;
                std::cout << plan << std::endl;
                // Unsubscribe so we only recieve one mission
                missionRawServer.unsubscribe_incoming_mission(handle);
                mission_prom.set_value(plan);
            });
    missionRawServer.subscribe_current_item_changed([](MissionRawServer::MissionItem item)
                                                    {
            std::cout << "Current Mission Item Changed!" << std::endl;
            std::cout << "Current Item: " << item << std::endl; });
    missionRawServer.subscribe_clear_all(
        [](uint32_t clear_all)
        { std::cout << "Clear All Mission!" << std::endl; });
    auto plan = mission_prom.get_future().get();

    // Set current item to complete to progress the current item state
    missionRawServer.set_current_item_complete();

    // As we're acting as an autopilot, lets just make the vehicle jump to 10m altitude on
    // successful takeoff
    actionServer.subscribe_takeoff([&position](ActionServer::Result result, bool takeoff)
                                   {
            if (result == ActionServer::Result::Success) {
                position.relative_altitude_m = 10;
            } });

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Publish the telemetry
        telemServer.publish_position(position, velocity, heading);
        telemServer.publish_position_velocity_ned(positionVelocityNed);
        telemServer.publish_raw_gps(rawGps, gpsInfo);
    }

    return 0;
}
