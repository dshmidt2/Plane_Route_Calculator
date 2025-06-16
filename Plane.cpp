#include "Plane.h"
#include "parser.h"
#include <cmath>
 
// Plane default constructor
Plane::Plane() {
   
    name_ = "No Name";
 
    IATA_ = "Invalid";
 
    max_distance_ = 0;
}
 
// Plane parameterized constructor
Plane::Plane(std::string name, std::string IATA) {
   
    setName(name);
 
    setIATA(IATA);
   
    max_distance_ = 0;
}
 
// Plane Name Setter
void Plane::setName(std::string name) {
    name_ = name;
}
 
// Plane Name Getter
std::string Plane::getName() {
    if (name_.empty()) {
        name_ = "No Name";
    }
    return name_;
}
 
// Plane IATA Code Setter
void Plane::setIATA(std::string IATA) {
    // If code is empty, set it to INVALID
    if (IATA.empty()) {
        IATA_ = "INVALID";
        return;
    }
 
    // If code is too long, set it to INVALID
    if (IATA.length() > 5) {
        IATA_ = "INVALID";
        return;
    }
 
    // If code is not the correct length, set it to INVALID
    if (IATA.length() != 5) {
        if (IATA.length() != 3) {
            IATA_ = "INVALID";
            return;
        }
    }
 
    // If code has length 5, trim it to length 3
    if (IATA.length() == 5) {
        std::string newIATA = IATA.substr(1,3);
        IATA_ = newIATA;
        return;
    }
 
    // Otherwise, set IATA_ = IATA
    IATA_ = IATA;
}
 
// Plane IATA Code Getter
std::string Plane::getIATA() {
    return IATA_;
}
 
// Plane Max Distance Setter
using namespace std;
void Plane::setMaxDistance(std::string IATA, const std::string& airportFile, const std::string& routeFile) {
 
    // If IATA code doesn't exist in database, set max_distance_ = 0
    if (IATA.length() != 3) {
        max_distance_ = 0;
        return;
    }
 
    // Update IATA_ if not already done so
    IATA_ = IATA;
 
    // Use Max Distance finder to set max distance for each IATA code plane.
     double longest = 0;
     double currentDistance = 0;
 
     Parser parser;
 
     std::vector<Airport>* airportList = parser.airportsParser(airportFile);
     std::vector<std::vector<std::string> >* routeData = parser.routeParser(routeFile);
 
     Airport departureA = (*airportList).at(0);
     Airport destinationA = (*airportList).at(0);
 
    // Loop through route data to find matching IATA code
    for (unsigned int x = 0; x < (unsigned int)(*routeData).size(); ++x) {
        for (unsigned int y = 3; y < (unsigned int)(*routeData).at(x).size(); ++y) {
   
            // If it matching IATA code and is a direct route, we check the distance between airports
            if ((*routeData).at(x).at(y) == IATA_ && ((*routeData).at(x).at(2) == "0")) {
 
                // get airport data using Airport vector
                for (unsigned int p = 0; p < (unsigned int)(*airportList).size(); ++p) {
                    if ((*airportList).at(p).getOAI() == (*routeData).at(x).at(0)) {
                        departureA = (*airportList).at(p);
                    }
                    if ((*airportList).at(p).getOAI() == (*routeData).at(x).at(1)) {
                        destinationA = (*airportList).at(p);
                    }
                }
 
                // get distance between airports
                currentDistance = rangeCalculator(departureA, destinationA);
 
                // if currentDistance > longest, set longest equal to currentDistance
                if (currentDistance > longest) {
                    longest = currentDistance;
                }
 
            }
 
        }
    }
 
    delete airportList;
    delete routeData;
 
    // Set max_distance_ equal to longest
    max_distance_ = longest;
}
 
// Plane Max Distance Getter
double Plane::getMaxDistance() {
    return max_distance_;
}
 
// Haversine function for calculating the distance of the route
double Plane::rangeCalculator(Airport & airportOne, Airport & airportTwo) const {
 
    double dLat = (airportTwo.getLat() - airportOne.getLat()) * M_PI / 180.0;
    double dLong = (airportTwo.getLong() - airportOne.getLong()) * M_PI / 180.0;
 
    double a = pow(sin(dLat/2), 2) + pow(sin(dLong/2), 2) * cos(airportOne.getLat() * M_PI / 180.0) * cos(airportTwo.getLat() * M_PI / 180.0);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
 
    return rad * c;
}

