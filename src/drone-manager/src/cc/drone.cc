#include "drone.hpp"

using namespace drone_manager;

Drone::Drone(const std::string m, const float s, const float a, const Coordenada c) : matricula(m), speed(s), altura(a), actual(c) {

}

std::string Drone::get_Matricula() const { return matricula; }

float Drone::get_Speed() const { return speed; }

float Drone::get_Altura() const { return altura; }

Coordenada Drone::get_Coordenda() const { return actual; }

std::shared_ptr<Route> Drone::get_Route() const { return route; }

/*******************************************************************************************************************************************/

void Drone::set_Matricula(const std::string &m) {
    matricula = m;
}

void Drone::set_Speed(const float &s) {
    speed = s;
}

void Drone::set_Altura(const float &a) { 
    altura = a;
}

void Drone::set_Coordenda(const Coordenada &c) {
    actual = c;
}



