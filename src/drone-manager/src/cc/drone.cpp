#include "drone.hpp"

using namespace drone_manager;

Drone::Drone(const std::string m, const Coordinate c, const float o) : matricula(m), actual(c), orientation(0), speed(0), height(0)
{
}

std::string Drone::get_Matricula() const { return matricula; }

float Drone::get_Speed() const { return speed; }

float Drone::get_Height() const { return height; }

Coordinate Drone::get_Coordinate() const { return actual; }

std::shared_ptr<Route> Drone::get_Route() const { return route; }

/*******************************************************************************************************************************************/

void Drone::set_Matricula(const std::string &m)
{
    matricula = m;
}

void Drone::set_Speed(const float &s)
{
    speed = s;
}

void Drone::set_Height(const float &a)
{
    height = a;
}

void Drone::set_Coordinate(const Coordinate &c)
{
    actual = c;
}
