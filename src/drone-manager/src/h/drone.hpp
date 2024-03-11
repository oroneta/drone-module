#ifndef __DRON_HPP__
#define __DRON_HPP__

#include <memory>
#include <string>
#include "route.hpp"


namespace drone_manager {
    
    class Drone {
    public:
        Drone(const std::string m, const float s, const float a, const Coordenada c);
        
        //GETTERS
        std::string get_Matricula() const;
        float get_Speed() const;
        float get_Altura() const;
        Coordenada get_Coordenda() const;
        std::shared_ptr<Route> get_Route() const;

        //SETTERS
        void set_Matricula(const std::string &m);
        void set_Speed(const float &s);
        void set_Altura(const float &a);
        void set_Coordenda(const Coordenada &c);
        void set_route(std::shared_ptr<Route> &r);

    private:
        std::string matricula;
        float speed;
        float altura;
        std::shared_ptr<Route> route;
        Coordenada actual;

    };
}


#endif  