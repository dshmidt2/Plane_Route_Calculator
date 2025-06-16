#include "Airport.h"

Airport::Airport(double longitude, double latitude, std::string name, std::string OAI) {
    longitude_ = longitude;
    latitude_ = latitude;

    name_ = name;

    OAI_ = OAI;
}

void Airport::setCoords(double longitude, double latitude) {
    longitude_ = longitude;
    latitude_ = latitude;
}

double Airport::getLong() {
    return longitude_;
}

double Airport::getLat() {
    return latitude_;
}

void Airport::setName(std::string name) {
    name_ = name;
}

std::string Airport::getName() {
    return name_;
}

void Airport::setOAI(std::string OAI) {
    OAI_ = OAI;
}
 std::string Airport::getOAI() {
     return OAI_;
 }