#ifndef __ROUTE_HPP__
#define __ROUTE_HPP__

#include <vector>

namespace drone_manager {
    class Coordinate {

        public:
            //Constructors
            Coordinate(const float x, const float y);
            Coordinate(const Coordinate &other);

            //Destructor
            ~Coordinate() {}

            //Operators
            bool operator==(const Coordinate &other) const;
            Coordinate& operator=(const Coordinate &other);

            //Getters
            float get_Latitude() const;
            float get_Longitude() const;

            //Setters
            void set_Latitude(const float value);
            void set_Longitude(const float value);

            //friend classes
            friend class Route;
        private:
            float latitude;
            float longitude;
    };

    class Route {

        public:
            
            //let compiler default constructor

            //Copy constructor
            Route(const Route &other);

            //Destructor
            ~Route() {}

            //Operators
            Route& operator=(const Route &other);

            //Getters
            Coordinate get_Start() const;
            Coordinate get_End() const;
            std::vector<Coordinate> get_Route() const;

            //Setters
            void set_Start(const Coordinate &value);
            void set_End(const Coordinate &value);
            void set_Route(const std::vector<Coordinate> &list);

            //other methods
            void push_back_point_to_route(const Coordinate &value);


        private:
            std::vector<Coordinate> coordinates;
            Coordinate start;
            Coordinate end;  
    };
}

#endif