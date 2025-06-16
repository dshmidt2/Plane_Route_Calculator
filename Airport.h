#pragma once

#include <iostream>
#include <string>

class Airport {
public:
    Airport(double x, double y, std::string name, std::string OAI);

    void setCoords(double x, double y);
    double getLong();
    double getLat();

    void setName(std::string name);
    std::string getName();

    void setOAI(std::string OAI);
    std::string getOAI();
private:
    //lat and long are given to 6s.f. so we use double (15 s.f.)
    //(float is 7 so we have some buffer for calculations)
    double longitude_;
    double latitude_;

    //name of the airport
    std::string name_;

    //Openflights Airport Identifier (OAI)
    std::string OAI_;
};