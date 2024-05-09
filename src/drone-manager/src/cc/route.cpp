#include "route.hpp"

using namespace drone_manager;

/**
 * Class Coordinate's functions definitions
 */
Coordinate::Coordinate(const float x, const float y) : latitude(x), longitude(y) {}

Coordinate::Coordinate(const Coordinate &other) : latitude(other.latitude), longitude(other.longitude) {}

// Operators
bool Coordinate::operator==(const Coordinate &other) const
{
    return (latitude == other.latitude && longitude == other.longitude);
}

Coordinate &Coordinate::operator=(const Coordinate &other)
{
    latitude = other.latitude;
    longitude = other.longitude;
    return *this;
}

// Getters
float Coordinate::get_Latitude() const { return latitude; }

float Coordinate::get_Longitude() const { return longitude; }

// Setters
void Coordinate::set_Latitude(const float value) { latitude = value; }

void Coordinate::set_Longitude(const float value) { longitude = value; }

/******************************************************************************************************************/

/**
 * Class Route's functions definitions
 */

// Constructors

// Operators
Route &Route::operator=(const Route &other)
{
    this->start = other.start;
    this->end = other.end;
    this->coordinates = other.coordinates;
    return *this;
}

// Getters
Coordinate Route::get_End() const
{
    return end;
}

Coordinate Route::get_Start() const
{
    return start;
}

std::vector<Coordinate> Route::get_Route() const
{
    return coordinates;
}

// Setters
void Route::set_Start(const Coordinate &value)
{
    start = value;
}

void Route::set_End(const Coordinate &value)
{
    start = value;
}

void Route::set_Route(const std::vector<Coordinate> &value)
{
    coordinates = value;
}

// Other methods
void Route::push_back_point_to_route(const Coordinate &value)
{
    coordinates.push_back(value);
}
