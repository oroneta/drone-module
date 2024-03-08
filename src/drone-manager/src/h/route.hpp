#ifndef __ROUTE_HPP__
#define __ROUTE_HPP__

#include <vector>

namespace drone_manager {
    class Coordenada {

        public:
            Coordenada(const float x, const float y);
            Coordenada(const Coordenada &other) : latitude(other.latitude), longitude(other.longitude) {}
            bool operator==(const Coordenada &other) const {
                return (latitude == other.latitude && longitude == other.longitude);
            }

            friend class Route;
        private:
            float latitude;
            float longitude;

    
    };

    class Route {

        public:

        private:
            std::vector<Coordenada> coordinates;
            Coordenada start;
            Coordenada end;  
    };
}

#endif