#ifndef __DRON_HPP__
#define __DRON_HPP__

#include <memory>
#include <string>
#include "route.hpp"


namespace dron_manager {
    class Dron {
    public:

        std::string get_Matricula() const;
        unsigned int get_Speed() const;
        int get_Altura() const;
        Coordenada get_Coordenda() const;

    private:
        std::string matricula;
        unsigned int speed;
        int altura;
        std::shared_ptr<Route> route;
        Coordenada actual;

    };
}


#endif  