#ifndef __DRON_HPP__
#define __DRON_HPP__

#include <memory>
#include <string>
#include "route.hpp"


namespace drone_manager {
    
    class Drone {
    public:
        Drone(const std::string m, const float s, const int a, const Coordenada c);
        std::string get_Matricula() const;
        float get_Speed() const;
        int get_Altura() const;
        Coordenada get_Coordenda() const;

    private:
        std::string matricula;
        float speed;
        int altura;
        std::shared_ptr<Route> route;
        Coordenada actual;

    };
}


#endif  