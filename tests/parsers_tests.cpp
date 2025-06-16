#include "../cs225/catch/catch.hpp"
#include "../parser.h"
 
 
/**** TEST AIRPORT PARSER ****/
TEST_CASE("test_airports_first_entry", "[parser_airport]") {
    Parser parser;
    std::vector<Airport>* vect = parser.airportsParser("tests/airports_tests.txt");
    REQUIRE("Goroka Airport" == (*vect)[0].getName());
    REQUIRE(-6.081689834590001 == (*vect)[0].getLat());
    REQUIRE(145.391998291 == (*vect)[0].getLong());
    REQUIRE("1" == (*vect)[0].getOAI());
}
 
TEST_CASE("test_airports_last_entry", "[parser_airport]") {
    Parser parser;
    std::vector<Airport>* vect = parser.airportsParser("tests/airports_tests.txt");
    REQUIRE("Kazan International Airport" == (*vect)[6].getName());
    REQUIRE(55.606201171875 == (*vect)[6].getLat());
    REQUIRE(49.278701782227 == (*vect)[6].getLong());
    REQUIRE("2990" == (*vect)[6].getOAI());
}
 
TEST_CASE("test_airports_with_comma", "[parser_airport]") {
    Parser parser;
    std::vector<Airport>* vect = parser.airportsParser("tests/airports_tests.txt");
    REQUIRE("Harstad/Narvik Airport, Evenes" == (*vect)[1].getName());
    REQUIRE(68.491302490234 == (*vect)[1].getLat());
    REQUIRE(16.678100585938 == (*vect)[1].getLong());
    REQUIRE("641" == (*vect)[1].getOAI());
}
 
// the line with invalid data (a string in latitude) is ignored,
// so the next line should be added instead
TEST_CASE("test_invalid_latitude", "[parser_airport]") {
    Parser parser;
    std::vector<Airport>* vect = parser.airportsParser("tests/airports_tests.txt");
    REQUIRE("Dawson Creek Airport" == (*vect)[2].getName());
    REQUIRE(55.7422981262207 == (*vect)[2].getLat());
    REQUIRE(-120.18299865722656 == (*vect)[2].getLong());
    REQUIRE("48" == (*vect)[2].getOAI());
}
 
// the line with invalid data (a string in longitude) is ignored,
// so the next line should be added instead
TEST_CASE("test_invalid_longitude", "[parser_airport]") {
    Parser parser;
    std::vector<Airport>* vect = parser.airportsParser("tests/airports_tests.txt");
    REQUIRE("Arviat Airport" == (*vect)[3].getName());
    REQUIRE(61.0942001343 == (*vect)[3].getLat());
    REQUIRE(-94.07080078119999 == (*vect)[3].getLong());
    REQUIRE("50" == (*vect)[3].getOAI());
}
 
 
/**** TEST VALID ROUTE PARSER ****/
TEST_CASE("test_valid_route", "[parser_route]") {
    Parser parser;
    std::map<std::pair<std::string, std::string>, int>* map = parser.validRouteParser("tests/routes_tests.txt");
    auto it = map->find(std::pair<std::string, std::string>("2965", "2990"));   // test valid route
    REQUIRE(it->first.first == "2965");
    REQUIRE(it->first.second == "2990");
}
 
TEST_CASE("test_invalid_route", "[parser_route]") {
    Parser parser;
    std::map<std::pair<std::string, std::string>, int>* map = parser.validRouteParser("tests/routes_tests.txt");
    auto it = map->find(std::pair<std::string, std::string>("2965", "INVALID"));   // test invalid route
    REQUIRE(it == map->end());
}
 
 
/**** TEST ROUTE PARSER ****/
TEST_CASE("test_route_first_entry", "[parser_route]") {
    Parser parser;
    std::vector<std::vector<std::string> >* routes = parser.routeParser("tests/routes_tests.txt");
    REQUIRE((*routes)[0][0] == "2965");
    REQUIRE((*routes)[0][1] == "2990");
    REQUIRE((*routes)[0][2] == "0");
    REQUIRE((*routes)[0][3] == "CR2");
}
 
// incomplete entry should be ignored, so the next line should be added instead
TEST_CASE("test_route_incomplete_entry", "[parser_route]") {
    Parser parser;
    std::vector<std::vector<std::string> >* routes = parser.routeParser("tests/routes_tests.txt");
    REQUIRE((*routes)[1][0] == "2966");
    REQUIRE((*routes)[1][1] == "2962");
    REQUIRE((*routes)[1][2] == "0");
    REQUIRE((*routes)[1][3] == "CR2");
}
 
// incomplete entry should be ignored, so the next line should be added instead
TEST_CASE("test_route_invaid_entry", "[parser_route]") {
    Parser parser;
    std::vector<std::vector<std::string> >* routes = parser.routeParser("tests/routes_tests.txt");
    REQUIRE((*routes)[2][0] == "2968");
    REQUIRE((*routes)[2][1] == "4078");
    REQUIRE((*routes)[2][2] == "0");
    REQUIRE((*routes)[2][3] == "CR2");
}
 
 
TEST_CASE("test_planeParser_valid_entry", "[parser_plane]") {
    // Parse the plane file
    Parser parser;
    std::vector<Plane>* planes = parser.planeParser("tests/planes.txt");

    // Make sure valid entries match expected outcome
    REQUIRE((*planes).at(0).getIATA() == "310");
    REQUIRE((*planes).at(0).getName() == "\"Airbus A310\"");
    REQUIRE((*planes).at(1).getIATA() == "318");
    REQUIRE((*planes).at(1).getName() == "\"Airbus A318\"");
}

TEST_CASE("test_planeParser_invalid_entry", "[parser_plane]") {
    // Parse the plane file
    Parser parser;
    std::vector<Plane>* planes = parser.planeParser("tests/planes.txt");

    // Code length too long should be INVALID
    REQUIRE((*planes).at(9).getIATA() == "INVALID");
    REQUIRE((*planes).at(9).getName() == "\"Code Length Too Long\"");
 
    // Code length too short should be INVALID
    REQUIRE((*planes).at(10).getIATA() == "INVALID");
    REQUIRE((*planes).at(10).getName() == "\"Code Length Too Short\"");
 
    // Null code should be INVALID
    REQUIRE((*planes).at(11).getIATA() == "INVALID");
    REQUIRE((*planes).at(11).getName() == "\"Code Doesn't Exist\"");
 
}

TEST_CASE("test_plane_class_getter", "[parser_plane]") {
    // Create new Plane object
    Plane* toTest = new Plane("Name", "COD");

    // Make sure the Plane getter returns expected outcome
    REQUIRE("Name" == (*toTest).getName());
    REQUIRE("COD" == (*toTest).getIATA());
 }
 
 TEST_CASE("test_plane_class_setter", "[parser_plane]") {
     // Create new Plane object
     Plane* toTest = new Plane("Name", "COD");
 
     // Set new Name and IATA code
     (*toTest).setName("New Name");
     (*toTest).setIATA("CO2");
 
     REQUIRE("New Name" == (*toTest).getName());
     REQUIRE("CO2" == (*toTest).getIATA());
 
     // Invalid code lengths should be INVALID
     (*toTest).setIATA("CO22");
     REQUIRE("INVALID" == (*toTest).getIATA());
     (*toTest).setIATA("CO");
     REQUIRE("INVALID" == (*toTest).getIATA());
 }
 
 TEST_CASE("test_plane_maxDistance", "[parser_plane]") {
    // Create new Plane object
    Plane* toTest = new Plane("Airbus A321", "321");
    (*toTest).setMaxDistance("321", "tests/airports_tests.txt", "tests/routes_tests.txt");
   
    // Check to see if Max Distance is between 0 and 6000 km as stated online for this plane model
    REQUIRE(0 <= (*toTest).getMaxDistance());
    REQUIRE(6000 > (*toTest).getMaxDistance());

    // Set to a different valid IATA code
    (*toTest).setIATA("CR2");
    (*toTest).setMaxDistance("CR2", "tests/airports_tests.txt", "tests/routes_tests.txt");

    // Check to see if Max Distance is between 0 and 3500 km as stated online for this plane model
    REQUIRE(3500 > (*toTest).getMaxDistance());

    // Set to an invalid IATA code
    (*toTest).setIATA("CO22");
    (*toTest).setMaxDistance("CO22", "tests/airports_tests.txt", "tests/routes_tests.txt");

    // Check to make sure that invalid code has a Max Distance of 0
    REQUIRE(0 == (*toTest).getMaxDistance());
}
