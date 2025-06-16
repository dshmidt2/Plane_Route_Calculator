#include "RouteMaker.h"
#include <map>
#include <iostream>
#include <queue>
#include <limits.h>
#include <algorithm>
#include <cmath>
#include <fstream>


RouteMaker::RouteMaker(std::vector<Airport>* airports, 
std::map<std::pair<std::string, std::string>, int>* validRoutes) {

    auto routeMakerStart = std::chrono::high_resolution_clock::now();
    airports_ = airports;

    std::map<std::string, size_t> airportOAIIndexMap;
    for (size_t i = 0; i < (*airports_).size(); i++) {
        airportOAIIndexMap.insert(std::pair<std::string, size_t>((*airports_)[i].getOAI(), i));
    } 

    matrix_ = std::vector<std::vector<double>>((*airports_).size(), std::vector<double>((*airports).size(), 0.0));
    for (auto pair : *validRoutes) {
        //first airport
        auto airportIndexOneIt = airportOAIIndexMap.find(pair.first.first);
        auto airportIndexTwoIt = airportOAIIndexMap.find(pair.first.second);

        if (airportIndexOneIt == airportOAIIndexMap.end() || airportIndexTwoIt == airportOAIIndexMap.end()) {
            continue;
        }
        
        matrix_[airportIndexOneIt->second][airportIndexTwoIt->second] = weightCalculator(airportIndexOneIt->second, airportIndexTwoIt->second);
    }

    delete validRoutes;
}

//empty constructor to facilitate unit function testing
RouteMaker::RouteMaker() {}

//function to help test BFS and Dijkstras unit tests
void RouteMaker::setMatrix(std::vector<std::vector<double>> matrix) {
    matrix_ = matrix;
}

//function to help test BFS and Dijkstras unit tests
void RouteMaker::setEdge(int i, int j, double edgeWeight) {
    //going from j to i
    matrix_[i][j] = edgeWeight;
    matrix_[j][i] = edgeWeight;
}

//destructor
RouteMaker::~RouteMaker() {
    if (airports_ != NULL) delete airports_;
}

//this is a haversine functions based off calculating distance for the lat and long
double RouteMaker::weightCalculator(const int airportOneIndex, const int airportTwoIndex) const {

    //take reference here so we dont waste time copying by value
    Airport& airportOne = (*airports_)[airportOneIndex];
    Airport& airportTwo = (*airports_)[airportTwoIndex];

    //if it still does not work we might try taking the abs value.

    double dLat = (airportTwo.getLat() - airportOne.getLat()) * M_PI / 180.0;
    double dLong = (airportTwo.getLong() - airportOne.getLong()) * M_PI / 180.0;

    double a = pow(sin(dLat/2), 2) + pow(sin(dLong/2), 2) * cos(airportOne.getLat() * M_PI / 180.0) * cos(airportTwo.getLat() * M_PI / 180.0);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;

    // return (airportOne.getLong() - airportTwo.getLong()) * (airportOne.getLong() - airportTwo.getLong())
    //  + (airportOne.getLat() - airportTwo.getLat()) * (airportOne.getLat() - airportTwo.getLat());
}

double RouteMaker::getVal(int i, int j){
    return matrix_[i][j];
}

int RouteMaker::indexFinder(std::string OAI) {
    for (int i = 0; i < (int)airports_->size(); i++) {
        if ((*airports_)[i].getOAI() == OAI) return i;
    }
    //if the airport is not found, if the OAI is invalid or anything we return -1
    //this is our error handling for wrong inputs
    return -1;
}

int RouteMaker::indexFinder(Airport airport) {
    for (int i = 0; i < (int)airports_->size(); i++) {
        if ((*airports_)[i].getOAI() == airport.getOAI()) return i;
    }
    //if the airport is not found, if the OAI is invalid or anything we return -1
    //this is our error handling for wrong inputs
    return -1;
}

//to be used by main when the user places his input
int RouteMaker::nameIndexFinder(std::string name) {
    for (int i = 0; i < (int)airports_->size(); i++) {
        if ((*airports_)[i].getName() == name) return i;
    }
    //if the airport is not found, if the OAI is invalid or anything we return -1
    //this is our error handling for wrong inputs
    return -1;
}

//standard BFS traversal for a adjacency matrix
std::vector<int> RouteMaker::BFS(const int indexOne, const int indexTwo) {

    //visited helps us track the path of our traversal from indexOne to indexTwo in the matrix
    std::vector<int> visited,ret; 
    std::queue<int> bfs;
    int cur_idx;

    //handling malformed input to the function
    if(indexOne<0||indexOne>=(int)airports_->size()||indexTwo<0||indexTwo>=(int)airports_->size()||indexOne==indexTwo)return ret;
    
    for(int i = 0; i < (int)airports_->size(); i++){
        visited.push_back(-1); //negative means not visited
    }

    visited[indexOne]=indexOne;
    bfs.push(indexOne);

    while(!bfs.empty()){
        cur_idx = bfs.front();
        bfs.pop();
        for(int i = 0; i < (int)airports_->size(); i++){
            if(visited[i]<0&&matrix_[cur_idx][i]!=0){
                visited[i]=cur_idx;
                bfs.push(i);
                if(i==indexTwo)break;
            }
        }
        if(bfs.back()==indexTwo)break;
    }

    //if we did not manage to visit the end we return an empty vector
    if(visited[indexTwo]==-1)return ret;

    //creating the path vector
    int bt_idx = indexTwo;
    while(bt_idx != indexOne){
        ret.push_back(bt_idx);
        bt_idx = visited[bt_idx];
    }
    ret.push_back(indexOne);
    reverse(ret.begin(), ret.end());
    return ret;
}

std::vector<int> RouteMaker::Dijkstra(const int indexStart, const int indexEnd, const double planeMaxDistance) {
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    //we use std::pair<weight, vertex> as the pair, and both are int

    //vector for distances
    std::vector<double> dist((matrix_).size(), INT_MAX);

    //vector for the path, this is indexed the same way as the airport
    //this is built in the same way that a disjoint set it, in the way that you are following
    //the index all the way until you reach -1
    //if it is -2 then that means that we dont touch it at all
    std::vector<int> path((matrix_).size(), -2);


    //start airport = indexStart, so we create it like this
    pq.push(std::pair<int, int>(0, indexStart));
    dist[indexStart] = 0;
    //we also update the path vector
    path[indexStart] = -1;

    while(!pq.empty()) {

        //we process the node with the least distance away
        int currentVertex = pq.top().second;
        pq.pop();

        //we have finished procrssing the shortest path to our destination, so we break
        if (currentVertex == indexEnd) break;

        //get all adjacent vertices of the current node
        for (size_t i = 0; i < matrix_.size(); i++) {
            //walking down the columns of the matrix to check for adjacent matrices

            if (matrix_[i][currentVertex] > 0 && matrix_[i][currentVertex] <= planeMaxDistance) {

                //the vertex we are testing against
                size_t testingVertex = i;

                //weight, checking if there is a shorter path to the testingVertex through the currentVertex
                double testingWeight = matrix_[testingVertex][currentVertex];
                
                //how we are calculating the shortest path in terms of distance
                if (dist[testingVertex] > dist[currentVertex] + testingWeight) {

                    //updating the distance
                    dist[testingVertex] = dist[currentVertex] + testingWeight;
                    pq.push(std::pair<int, int>(dist[testingVertex], testingVertex));

                    //updating the path as well, so it points at the new vertex
                    if (path[testingVertex] != -1) path[testingVertex] = currentVertex;
                }
            }
        }
    }

    //handling case for if we cannot reach the end
    if (path[indexEnd] == -2) return std::vector<int>();

    //getting the path to return
    std::vector<int> returnVect;
    Dhelper(returnVect, path, indexEnd);
    reverse(returnVect.begin(), returnVect.end());

    // std::cout << "the total flight distance is: " << dist[indexEnd] << " kilometers." << std::endl;
    return returnVect;
}

//a recursive helper function like we implemented in disjoint sets to record the path
void RouteMaker::Dhelper(std::vector<int> & returnVect, const std::vector<int> & pathVect, int currIndex) {
    if (pathVect[currIndex] == -2) std::cout << "this really should not happen" << std::endl;
    
    //push back the current one we are at
    returnVect.push_back(currIndex);

    //the beginning we have marked as -1
    if (pathVect[currIndex] == -1) return;

    //we recursively call Dhelper
    Dhelper(returnVect, pathVect, pathVect[currIndex]);
}

void RouteMaker::output(const std::vector<int> path) {
    if (path.size() == 0) std::cout << "there is no valid route between the two airports" << std::endl;

    //source airport
    else{
        std::cout << "starting at: " << (*airports_)[path[0]].getName();
        for (size_t i = 1; i < path.size(); i++) {
            std::cout << "-->" << (*airports_)[path[i]].getName();
        }
        //destination airport
        std::cout << " is the route you should take." << std::endl;
    }
}

//debugging help function
void RouteMaker::printEdge(int i, int j) {
    std::cout << "ij: " << matrix_[i][j] << "ji: " << matrix_[j][i] << std::endl;
}

//function to be used in main
void RouteMaker::outputAirports() {
    std::ofstream outfile ("airports.txt");
    for (auto elem : *airports_) outfile << elem.getName() << std::endl;
    outfile.close();
}