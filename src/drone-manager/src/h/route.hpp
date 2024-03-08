#ifndef __ROUTE_HPP__
#define __ROUTE_HPP__

#include <vector>

namespace drone_manager {
    class Coordenada {

        public:
            Coordenada(const float x, const float y);
            
            Coordenada(const Coordenada &other);

            bool operator==(const Coordenada &other) const;

            float get_Latitude() const;

            float get_Longitude() const;

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