#pragma once

#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include "Airport.h"
#include "Plane.h"

class RouteMaker {
public:

    RouteMaker(std::vector<Airport>* airports, std::map<std::pair<std::string, std::string>, int>* validRoutes);
    //empty one for unit function testing
    RouteMaker();
    void setMatrix(std::vector<std::vector<double>> matrix);
    void setEdge(int i, int j, double edgeWeight);
    ~RouteMaker();

    double getVal(int i, int j); // getter function to get matrix val
    //this returns the square of the euclidean distance between the two airports  
    double weightCalculator(const int airportOneIndex, const int airportTwoIndex) const;
    int indexFinder(std::string OAI);
    int indexFinder(Airport airport);
    int nameIndexFinder(std::string name);
    std::vector<int> BFS(const int indexOne, const int indexTwo);
    std::vector<int> Dijkstra(const int indexStart, const int indexEnd, const double planeMaxDistance);
    void Dhelper(std::vector<int> & returnVect, const std::vector<int> & pathVect, int indexStart);
    void output(const std::vector<int> path);
    void printEdge(int i, int j);
    void outputAirports();
    
private:


    //our adjacency matrix, values are the weight
    //matrix is 0 indexed the same way as the airports
    std::vector<std::vector<double> > matrix_;
    
    //our vector of all the airports, pointer bc we take over ownership from parser
    std::vector<Airport>* airports_ = NULL;
};