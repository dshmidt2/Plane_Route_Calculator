#include <iostream>
#include "parser.h"
// #include "edge.h"
#include "RouteMaker.h"
#include <chrono>
#include "line_drawing.cpp"
#include <limits.h>


int main() {
    // auto mainStart = std::chrono::high_resolution_clock::now();

    //let the user choose the files they want to use it with
    std::string airportsFileName = "data/datasets/airports.dat.txt";
    std::string routesFileName = "data/datasets/routes.dat.txt";
    std::string planesFileName = "data/datasets/planes.dat.txt";

    std::cout << "please input the path to your airports dataset. if you press enter, we will default to the standard dataset: \"data/datasets/airports.dat.txt\" " << std::endl;
    getline(std::cin, airportsFileName);
    if (airportsFileName.empty()) airportsFileName = "data/datasets/airports.dat.txt";

    std::cout << "please input the path to your routes dataset. if you press enter, we will default to the standard dataset: \"data/datasets/routes.dat.txt\" " << std::endl;
    getline(std::cin, routesFileName);
    if (routesFileName.empty()) routesFileName = "data/datasets/routes.dat.txt";

    std::cout << "please input the path to your planes dataset. if you press enter, we will default to the standard dataset: \"data/datasets/planes.dat.txt\" " << std::endl;
    getline(std::cin, planesFileName);
    if (planesFileName.empty()) planesFileName = "data/datasets/planes.dat.txt";

    //create the output for the users to take a look at what they have to use
    std::cout << "algorithm started" << std::endl;
    Parser parser;
    std::cout << "parsing airports..." << std::endl;
    std::vector<Airport>* airports = parser.airportsParser(airportsFileName);
    std::cout << "parsing routes..." << std::endl;
    std::map<std::pair<std::string, std::string>, int>* validRoutes = parser.validRouteParser(routesFileName);
    std::cout << "parsing airplanes..." << std::endl;
    std::vector<Plane>* airplanes = parser.planeParser(planesFileName);
    std::cout << "creating RouteMaker object" << std::endl;
    RouteMaker test = RouteMaker(airports, validRoutes);

    std::cout << "outputing airplanes to airplanes.txt" << std::endl;
    std::ofstream outfilePlane ("airplanes.txt");
    for (auto elem : *airplanes) outfilePlane << elem.getName() << std::endl;
    outfilePlane.close();
    std::cout << "outputing airports to airports.txt" << std::endl;
    std::ofstream outfileAirport ("airports.txt");
    for (auto elem : *airports) outfileAirport << elem.getName() << std::endl;
    outfileAirport.close();

    std::cout << "\n";

    //user starts inputting the values he wants. while loops to account for malformed inputs
    std::cout << "please choose a start airport from airports.txt" << std::endl;
    std::string startAirport;
    getline(std::cin, startAirport);
    int startAirportIndex = test.nameIndexFinder(startAirport);
    while (startAirportIndex == -1) {
        std::cout << "you chose " << startAirport << std::endl;
        std::cout << "This start airport in not in airports.txt. Please choose another one." << std::endl;
        getline(std::cin, startAirport);
        startAirportIndex = test.nameIndexFinder(startAirport);
    }

    std::cout << "\n";

    std::cout << "please choose a destination airport from airports.txt" << std::endl;
    std::string destAirport;
    getline(std::cin, destAirport);
    int destAirportIndex = test.nameIndexFinder(destAirport);
    while (destAirportIndex == -1) {
        std::cout << "you chose " << destAirport << std::endl;
        std::cout << "This destination airport in not in airports.txt. Please choose another one." << std::endl;
        getline(std::cin, destAirport);
        destAirportIndex = test.nameIndexFinder(destAirport);
    }
    
    std::cout << "\n";

    std::cout << "please choose a plane from airplanes.txt (please include the colons)" << std::endl;
    std::string airplaneName;
    Plane airplane;

    bool planeExists = false;
    bool planeRange = false;
    do {
        planeExists = false;

        getline(std::cin, airplaneName);

        for (auto elemPlane : *airplanes) {
            if (elemPlane.getName() == airplaneName) {
                planeExists = true;
                airplane = elemPlane;
                airplane.setMaxDistance(airplane.getIATA(), "data/datasets/airports.dat.txt", "data/datasets/routes.dat.txt");
                if (airplane.getMaxDistance() != 0) planeRange = true;
            }
        }
        if (!planeExists) {
            std::cout << "this airplane does not exist. please choose another one." << std::endl;
        } else if (!planeRange) {
            std::cout << "this airplane has not yet flown any routes in the dataset, and therefore we don't know its range based on the dataset. please choose another plane" << std::endl;
        }

    } while (!planeExists || !planeRange);

    std::cout << "you have chosen a plane whose max range is: " << airplane.getMaxDistance() << " kilometers based on the routes it has flown in our dataset." << std::endl;

    std::cout << "\n";

    std::cout << "please enter what you want the PNG with the route overlaid on a world map to be called: {your_input}.png." << std::endl;
    std::string outputFileName;
    getline(std::cin, outputFileName);
    outputFileName += ".png";

    std::cout << "\n";

    //calculation for shortest path starts
    std::vector<int> path = test.Dijkstra(startAirportIndex, destAirportIndex, airplane.getMaxDistance());
    // std::vector<int> path = test.Dijkstra(startAirportIndex, destAirportIndex, INT_MAX);
    std::cout << "here is the route you should take: " << std::endl;
    test.output(path);

    std::cout << "\n";

    std::cout << "drawing route on world map..." << std::endl;
    cs225::PNG* result = drawLine(path, airports);
    if (result != NULL) {
        result->writeToFile(outputFileName);
        std::cout << "you may view the route projected onto the world map at " << outputFileName << std::endl;
        delete result;
    }

    delete airplanes;

    // auto mainStop = std::chrono::high_resolution_clock::now();
    // auto mainDuration = std::chrono::duration_cast<std::chrono::seconds>(mainStop - mainStart);
    // std::cout << "Time of algorithm in seconds: " << abs(mainDuration.count()) << std::endl;

    return 0;

}