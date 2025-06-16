#pragma once
 
#include <iostream>
#include <string>
#include <vector>
#include "Airport.h"
 
class Plane {
public:
   
    // Plane Default Constructor
    Plane();
 
    // Plane Parameterized Constructor
    Plane(std::string name, std::string IATA);
 
    // Plane Name Setter and Getter
    void setName(std::string name);
    std::string getName();
 
    // Plane IATA Code Setter and Getter
    void setIATA(std::string IATA);
    std::string getIATA();
 
    // Plane Max Distance Setter and Getter
    void setMaxDistance(std::string IATA, const std::string& airportFile, const std::string& routeFile);
    double getMaxDistance();
 
    // Haversine function for calculating the distance of the route
    double rangeCalculator(Airport & airportOne, Airport & airportTwo) const;
 
 
 
private:
 
    // Name of the plane
    std::string name_;
 
    // Openflights Three Letter Identifier Code
    std::string IATA_;
 
    // Maximum distance the plane can travel
    double max_distance_;
};
 
 

