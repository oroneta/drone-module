#include "route.hpp"

using namespace drone_manager;

/**
 * Definición de los metodos de la clase Coordenada
*/
Coordenada::Coordenada(const float x, const float y) : latitude(x), longitude(y) {}
Coordenada::Coordenada(const Coordenada &other) : latitude(other.latitude), longitude(other.longitude) {}

bool Coordenada::operator==(const Coordenada &other) const {
    return (latitude == other.latitude && longitude == other.longitude);
}

float Coordenada::get_Latitude() const { return latitude; }

float Coordenada::get_Longitude() const { return longitude; }