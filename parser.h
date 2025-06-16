#pragma once
#include "Airport.h"
#include "Plane.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstring>
#include <stdexcept>

//parses the data from the datasets
class Parser {
public:
    std::vector<Airport>* airportsParser(const std::string& filename);
    std::map<std::pair<std::string, std::string>, int>* validRouteParser(const std::string& filename);
    std::vector<std::vector<std::string> >* routeParser(const std::string& filename);
    std::vector<Plane>* planeParser(const std::string& filename);
private:

};