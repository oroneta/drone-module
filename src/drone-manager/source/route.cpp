#include "route.hpp"

using namespace dron_manager;

/**
 * Definición de los metodos de la clase Coordenada
*/
Coordenada::Coordenada(const float x, const float y) : latitude(x), longitude(y) {}
Coordenada::Coordenada(const Coordenada &other) : latitude(other.latitude), longitude(other.longitude) {}

bool Coordenada::operator==(const Coordenada &other) const {
                return (latitude == other.latitude && longitude == other.longitude);
    }