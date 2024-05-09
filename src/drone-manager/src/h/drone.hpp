#ifndef __DRON_HPP__
#define __DRON_HPP__

#include <memory>
#include <string>
#include "route.hpp"

/**********************************************************************************************************/
// Movement codes
// 1XX
#define DRONE_NORMAL_UP 100
#define DRONE_SLOW_UP 101
#define DRONE_FAST_UP 102

// 2xx
#define DRONE_NORMAL_DOWN 200
// #define DRONE_SLOW_DOWN 201
// #define DRONE_FAST_DOWN 202

// 3xx
#define DRONE_NORMAL_LEFT 300
// #define DRONE_SLOW_LEFT 301
// #define DRONE_FAST_LEFT 302

// 4xx
#define DRONE_NORMAL_RIGHT 400
// #define DRONE_SLOW_RIGHT 401
// #define DRONE_FAST_RIGHT 402

// 5xx
#define DRONE_NORMAL_FRONT 500
// #define DRONE_SLOW_FRONT 501
// #define DRONE_FAST_FRONT 502

// 6xx
#define DRONE_NORMAL_BACK 600
// #define DRONE_SLOW_BACK 601
// #define DRONE_FAST_BACK 602

// 7xx
#define DRONE_STOP 700

/*********************************************************************************************************/

namespace drone_manager
{

    class Drone
    {
    public:
        // Constructor
        Drone(const std::string m, const Coordinate c, const float o);

        // Destructor
        ~Drone() {}

        // GETTERS
        std::string get_Matricula() const;
        float get_Speed() const;
        float get_Height() const;
        Coordinate get_Coordinate() const;
        std::shared_ptr<Route> get_Route() const;

        // SETTERS
        void set_Matricula(const std::string &m);
        void set_Speed(const float &s);
        void set_Height(const float &a);
        void set_Coordinate(const Coordinate &c);
        void set_route(std::shared_ptr<Route> &r);

    private:
        std::string matricula;
        std::shared_ptr<Route> route;
        Coordinate actual;
        float orientation;
        float speed;
        float height;
    };
}

#endif