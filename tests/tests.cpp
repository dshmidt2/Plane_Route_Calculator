#include "../cs225/catch/catch.hpp"

#include "../RouteMaker.h"
#include "../parser.h"
#include <limits.h>

std::vector<Airport>* create_airport(){
    std::vector<Airport>* airports=new std::vector<Airport>;
    airports->push_back(Airport(51.4706,-0.461941,"London Heathrow Airport","507"));
    airports->push_back(Airport(68.534401,-89.808098,"Kugaaruk Airport","26"));
    airports->push_back(Airport(28.200899124145508,83.98210144042969,"Pokhara Airport","3127"));
    airports->push_back(Airport(53.552776,10.006683,"Hamburg Hbf","8810"));
    airports->push_back(Airport(65.66000366210938,-18.07270050048828,"Akureyri Airport","11"));
    return  airports;
};


std::map<std::pair<std::string, std::string>, int>* create_valid(){
    std::map<std::pair<std::string, std::string>, int>* validRoutes=new std::map<std::pair<std::string, std::string>, int>;
    std::pair <std::string, std::string> f1 ("507","26");//i,j=0,1
    std::pair <std::string, std::string> v1 ("26","507");//i,j=1,0
    std::pair <std::string, std::string> f2 ("507","3127");//i,j=0,2
    std::pair <std::string, std::string> v2 ("3127","507");//i,j=2,0
    std::pair <std::string, std::string> f3 ("8810","26");//i,j=3,1
    std::pair <std::string, std::string> v3 ("26","8810");//i,j=1,3
    std::pair <std::string, std::string> f4 ("8810","11");//i,j=3,4
    std::pair <std::string, std::string> v4 ("11","8810");//i,j=4,3
    validRoutes->insert({f1,1});
    validRoutes->insert({f2,1});
    validRoutes->insert({f3,1});
    validRoutes->insert({f4,1});
    validRoutes->insert({v1,1});
    validRoutes->insert({v2,1});
    validRoutes->insert({v3,1});
    validRoutes->insert({v4,1});
    return validRoutes;
};



TEST_CASE("test_airport_class_getter", "[Airport]") {
    std::vector<Airport>* airports=create_airport();


    REQUIRE(51.4706 == (*airports)[0].getLong());
    REQUIRE(-0.461941 ==(*airports)[0].getLat());
    REQUIRE("London Heathrow Airport" ==(*airports)[0].getName());
    REQUIRE("507" ==(*airports)[0].getOAI());

}

TEST_CASE("test_airport_class_setter", "[Airport]") {
    std::vector<Airport>* airports=create_airport();
    (*airports)[4].setCoords(-6.081689834590001,145.391998291);
    (*airports)[4].setName("Goroka Airport");
    (*airports)[4].setOAI("1");
    REQUIRE(-6.081689834590001 ==(*airports)[4].getLong());
    REQUIRE(145.391998291 ==(*airports)[4].getLat());
    REQUIRE("Goroka Airport" ==(*airports)[4].getName());
    REQUIRE("1" ==(*airports)[4].getOAI());
}


TEST_CASE("weight calculator", "[weightCalculator]") {
    std::vector<Airport>* airports=create_airport();
    std::map<std::pair<std::string, std::string>, int>* validRoutes=create_valid();
    RouteMaker rm(airports,validRoutes);

    std::vector<double> correctValues{9936,9444,1187,2498,19330,11100,7977,8292,11482,3393};

    int count = 0;
    for(unsigned i=0;i<airports->size();i++){
        for(unsigned j=i+1;j<airports->size();j++){
            REQUIRE(round(rm.weightCalculator(i,j)) == correctValues[count]);
            count++;
        }
    }
}

TEST_CASE("test_RouteMaker", "[RouteMaker]") {
    std::vector<Airport>* airports=create_airport();
    std::map<std::pair<std::string, std::string>, int>* validRoutes=create_valid();
    RouteMaker rm(airports,validRoutes);
    REQUIRE(rm.getVal(0,0) == 0);
    REQUIRE(rm.getVal(0,1) == rm.weightCalculator(0,1));
    REQUIRE(rm.getVal(0,2) == rm.weightCalculator(0,2));
    REQUIRE(rm.getVal(1,3) == rm.weightCalculator(1,3));
    REQUIRE(rm.getVal(3,1) == rm.weightCalculator(3,1));
    REQUIRE(rm.getVal(3,4) == rm.weightCalculator(3,4));
    REQUIRE(rm.getVal(1,4) == 0);
    REQUIRE(rm.getVal(4,4) == 0);
}

TEST_CASE("test_index_finder", "[indexFinder]") {
    std::vector<Airport>* airports=create_airport();
    airports->push_back(Airport(66.05809783935547,-23.135299682617188,"Ísafjörður Airport","15"));
    std::map<std::pair<std::string, std::string>, int>* validRoutes =create_valid();
    RouteMaker rm(airports,validRoutes);

    REQUIRE(rm.indexFinder(Airport(51.4706,-0.461941,"London Heathrow Airport","507")) == 0);
    REQUIRE(rm.indexFinder(Airport(68.534401,-89.808098,"Kugaaruk Airport","26")) == 1);
    REQUIRE(rm.indexFinder(Airport(28.200899124145508,83.98210144042969,"Pokhara Airport","3127")) == 2);
    REQUIRE(rm.indexFinder(Airport(53.552776,10.006683,"Hamburg Hbf","8810")) == 3);
    REQUIRE(rm.indexFinder(Airport(65.66000366210938,-18.07270050048828,"Akureyri Airport","11")) == 4);
    REQUIRE(rm.indexFinder(Airport(66.05809783935547,-23.135299682617188,"Ísafjörður Airport","15")) == 5);
    REQUIRE(rm.indexFinder("507") == 0);
    REQUIRE(rm.indexFinder("26") == 1);
    REQUIRE(rm.indexFinder("3127") == 2);
    REQUIRE(rm.indexFinder("8810") == 3);
    REQUIRE(rm.indexFinder("11") == 4);
    REQUIRE(rm.indexFinder("15") == 5);
}

TEST_CASE("test_BFS", "[BFS]") {
    std::vector<Airport>* airports=create_airport();
    airports->push_back(Airport(66.05809783935547,-23.135299682617188,"Ísafjörður Airport","15"));//i=5
    std::map<std::pair<std::string, std::string>, int>* validRoutes=create_valid();
    RouteMaker rm(airports,validRoutes);

    REQUIRE(rm.BFS(-1,5) == std::vector<int>{});
    REQUIRE(rm.BFS(0,4) == std::vector<int>{0,1,3,4});
    REQUIRE(rm.BFS(0,2) == std::vector<int>{0,2});
    REQUIRE(rm.BFS(1,2) == std::vector<int>{1,0,2});
    REQUIRE(rm.BFS(2,4) == std::vector<int>{2,0,1,3,4});
    REQUIRE(rm.BFS(2,5) == std::vector<int>{});
}

TEST_CASE("medium graph BFS", "[BFS]") {
    std::vector<Airport>* airports=create_airport();
    airports->push_back(Airport(66.05809783935547,-23.135299682617188,"Ísafjörður Airport","15"));//i=5
    airports->push_back(Airport(70.4860992432,-68.5167007446,"Clyde River Airport","40"));//6
    airports->push_back(Airport(79.9946975708,-85.814201355,"Eureka Airport","53"));//7
    airports->push_back(Airport(68.30419921880001,-133.483001709,"Inuvik Mike Zubko Airport","54"));//8
    airports->push_back(Airport(63.756402,-68.555801,"Iqaluit Airport","55"));//9
    airports->push_back(Airport(45.868900299072266,-66.53720092773438,"Fredericton Airport","56"));//10
    std::map<std::pair<std::string, std::string>, int>* validRoutes=create_valid();
    std::pair <std::string, std::string> f1 ("55","53");//i,j=9,7
    std::pair <std::string, std::string> v1 ("15","507");//i,j=5,0
    std::pair <std::string, std::string> f2 ("56","3127");//i,j=10,2
    std::pair <std::string, std::string> v2 ("8810","56");//i,j=3,10
    std::pair <std::string, std::string> f3 ("54","40");//i,j=8,6
    std::pair <std::string, std::string> v3 ("40","8810");//i,j=6,3
    std::pair <std::string, std::string> f4 ("55","11");//i,j=9,4
    std::pair <std::string, std::string> v4 ("55","8810");//i,j=9,3
    validRoutes->insert({f1,1});
    validRoutes->insert({f2,1});
    validRoutes->insert({f3,1});
    validRoutes->insert({f4,1});
    validRoutes->insert({v1,1});
    validRoutes->insert({v2,1});
    validRoutes->insert({v3,1});
    validRoutes->insert({v4,1});
    RouteMaker rm(airports,validRoutes);

    REQUIRE(rm.BFS(-1,9) == std::vector<int>{});
    REQUIRE(rm.BFS(9,2) == std::vector<int>{9,3,10,2});
    REQUIRE(rm.BFS(5,4) == std::vector<int>{5,0,1,3,4});
    REQUIRE(rm.BFS(10,4) == std::vector<int>{ 10, 2, 0, 1, 3, 4 });
    REQUIRE(rm.BFS(2,7) == std::vector<int>{});
    REQUIRE(rm.BFS(6,4) == std::vector<int>{6, 3, 4});
    REQUIRE(rm.BFS(6,2) == std::vector<int>{6, 3, 10, 2 });
}

TEST_CASE("single edge dijkstra's", "[dijkstra]") {
    //create a custom small graph making sure that the path found is correct
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(6, std::vector<double>(6, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    //setting the edges for that graph
    test.setEdge(0, 1, 2.0);
    test.setEdge(0, 2, 3.0);
    test.setEdge(0, 3, 4.0);
    test.setEdge(1, 3, 3.0);
    test.setEdge(3, 4, 6.0);
    test.setEdge(0, 5, 15.0);
    test.setEdge(4, 5, 2.0);

    std::vector<int> pathOne = test.Dijkstra(0, 3, INT_MAX);
    REQUIRE((int)pathOne.size() == 2);
    REQUIRE((int)pathOne[0] == 0);
    REQUIRE((int)pathOne[1] == 3);

    std::vector<int> pathTwo = test.Dijkstra(0, 1, INT_MAX);
    REQUIRE((int)pathTwo.size() == 2);
    REQUIRE((int)pathTwo[0] == 0);
    REQUIRE((int)pathTwo[1] == 1);

    std::vector<int> pathThree = test.Dijkstra(1, 3, INT_MAX);
    REQUIRE((int)pathThree.size() == 2);
    REQUIRE((int)pathThree[0] == 1);
    REQUIRE((int)pathThree[1] == 3);

    std::vector<int> pathFour = test.Dijkstra(5, 4, INT_MAX);
    REQUIRE((int)pathFour.size() == 2);
    REQUIRE((int)pathFour[0] == 5);
    REQUIRE((int)pathFour[1] == 4);

    std::vector<int> pathFive = test.Dijkstra(4, 3, INT_MAX);
    REQUIRE((int)pathFive.size() == 2);
    REQUIRE((int)pathFive[0] == 4);
    REQUIRE((int)pathFive[1] == 3);
}

TEST_CASE("small graph dijkstra's unlimited", "[dijkstra]") {
    //create a custom small graph making sure that the path found is correct
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(6, std::vector<double>(6, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    //setting the edges for that graph
    test.setEdge(0, 1, 2.0);
    test.setEdge(0, 2, 3.0);
    test.setEdge(0, 3, 4.0);
    test.setEdge(1, 3, 3.0);
    test.setEdge(3, 4, 6.0);
    test.setEdge(0, 5, 15.0);
    test.setEdge(4, 5, 2.0);

    std::vector<int> pathOne = test.Dijkstra(0, 4, INT_MAX);
    REQUIRE((int)pathOne.size() == 3);
    REQUIRE((int)pathOne[0] == 0);
    REQUIRE((int)pathOne[1] == 3);
    REQUIRE((int)pathOne[2] == 4);

    std::vector<int> pathTwo = test.Dijkstra(0, 5, INT_MAX);
    REQUIRE((int)pathTwo.size() == 4);
    REQUIRE((int)pathTwo[0] == 0);
    REQUIRE((int)pathTwo[1] == 3);
    REQUIRE((int)pathTwo[2] == 4);
    REQUIRE((int)pathTwo[3] == 5);
}

TEST_CASE("small graph dijkstra's with restriction", "[dijkstra]") {
    //create a custom small graph making sure that the path found is correct
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(6, std::vector<double>(6, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    //setting the edges for that graph
    test.setEdge(0, 1, 2.0);
    test.setEdge(0, 2, 3.0);
    test.setEdge(0, 3, 4.0);
    test.setEdge(1, 3, 3.0);
    test.setEdge(3, 4, 6.0);
    test.setEdge(0, 5, 15.0);
    test.setEdge(4, 5, 2.0);

    std::vector<int> pathOne = test.Dijkstra(0, 3, 3);
    REQUIRE((int)pathOne.size() == 3);
    REQUIRE((int)pathOne[0] == 0);
    REQUIRE((int)pathOne[1] == 1);
    REQUIRE((int)pathOne[2] == 3);
}

TEST_CASE("medium graph dijkstra's unlimited", "[dijkstra]") {
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(16, std::vector<double>(16, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    test.setEdge(0, 1, 1.0);
    test.setEdge(0, 14, 1.0);
    test.setEdge(0, 3, 5.0);
    test.setEdge(0, 2, 1.0);
    test.setEdge(0, 9, 12.0);
    test.setEdge(0, 4, 3.0);
    test.setEdge(1, 5, 2.0);
    test.setEdge(2, 6, 3.0);
    test.setEdge(3, 10, 4.0);
    test.setEdge(4, 12, 3.0);
    test.setEdge(5, 7, 3.0);
    test.setEdge(6, 8, 2.0);
    test.setEdge(6, 11, 6.0);
    test.setEdge(7, 10, 4.0);
    test.setEdge(9, 11, 3.0);
    test.setEdge(9, 12, 5.0);
    test.setEdge(12, 15, 3.0);

    std::vector<int> pathOne = test.Dijkstra(0, 5, INT_MAX);
    REQUIRE((int)pathOne.size() == 3);
    REQUIRE(pathOne[0] == 0);
    REQUIRE(pathOne[1] == 1);
    REQUIRE(pathOne[2] == 5);

    std::vector<int> pathTwo = test.Dijkstra(0, 8, INT_MAX);
    REQUIRE((int)pathTwo.size() == 4);
    REQUIRE(pathTwo[0] == 0);
    REQUIRE(pathTwo[1] == 2);
    REQUIRE(pathTwo[2] == 6);
    REQUIRE(pathTwo[3] == 8);

    std::vector<int> pathThree = test.Dijkstra(0, 10, INT_MAX);
    REQUIRE((int)pathThree.size() == 3);
    REQUIRE(pathThree[0] == 0);
    REQUIRE(pathThree[1] == 3);
    REQUIRE(pathThree[2] == 10);

    std::vector<int> pathFour = test.Dijkstra(0, 11, INT_MAX);
    REQUIRE((int)pathFour.size() == 4);
    REQUIRE(pathFour[0] == 0);
    REQUIRE(pathFour[1] == 2);
    REQUIRE(pathFour[2] == 6);
    REQUIRE(pathFour[3] == 11);

    std::vector<int> pathFive = test.Dijkstra(0, 9, INT_MAX);
    REQUIRE((int)pathFive.size() == 4);
    REQUIRE(pathFive[0] == 0);
    REQUIRE(pathFive[1] == 4);
    REQUIRE(pathFive[2] == 12);
    REQUIRE(pathFive[3] == 9);
}

TEST_CASE("medium graph dijkstra's with restriction", "[dijkstra]") {
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(16, std::vector<double>(16, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    test.setEdge(0, 1, 1.0);
    test.setEdge(0, 14, 1.0);
    test.setEdge(0, 3, 5.0);
    test.setEdge(0, 2, 1.0);
    test.setEdge(0, 9, 12.0);
    test.setEdge(0, 4, 3.0);
    test.setEdge(1, 5, 2.0);
    test.setEdge(2, 6, 3.0);
    test.setEdge(3, 10, 4.0);
    test.setEdge(4, 12, 3.0);
    test.setEdge(5, 7, 3.0);
    test.setEdge(6, 8, 2.0);
    test.setEdge(6, 11, 6.0);
    test.setEdge(7, 10, 4.0);
    test.setEdge(9, 11, 3.0);
    test.setEdge(9, 12, 5.0);
    test.setEdge(12, 15, 3.0);

    //restriction = 4, 0 to 10
    std::vector<int> pathOne = test.Dijkstra(0, 10, 4);
    REQUIRE((int)pathOne.size() == 5);
    REQUIRE(pathOne[0] == 0);
    REQUIRE(pathOne[1] == 1);
    REQUIRE(pathOne[2] == 5);
    REQUIRE(pathOne[3] == 7);
    REQUIRE(pathOne[4] == 10);

    //restriction = 6, 0 to 9
    std::vector<int> pathTwo = test.Dijkstra(0, 9, 6);
    REQUIRE((int)pathTwo.size() == 4);
    REQUIRE(pathTwo[0] == 0);
    REQUIRE(pathTwo[1] == 4);
    REQUIRE(pathTwo[2] == 12);
    REQUIRE(pathTwo[3] == 9);

    //restriction = 6, 0 to 11
    std::vector<int> pathThree = test.Dijkstra(0, 11, 6);
    REQUIRE((int)pathThree.size() == 4);
    REQUIRE(pathThree[0] == 0);
    REQUIRE(pathThree[1] == 2);
    REQUIRE(pathThree[2] == 6);
    REQUIRE(pathThree[3] == 11);

    //restriction = 5, 0 to 11
    std::vector<int> pathFour = test.Dijkstra(0, 11, 5);
    REQUIRE((int)pathFour.size() == 5);
    REQUIRE(pathFour[0] == 0);
    REQUIRE(pathFour[1] == 4);
    REQUIRE(pathFour[2] == 12);
    REQUIRE(pathFour[3] == 9);
    REQUIRE(pathFour[4] == 11);

    //restriction = 5, 0 to 10
    std::vector<int> pathFive = test.Dijkstra(0, 10, 5);
    REQUIRE((int)pathFive.size() == 3);
    REQUIRE(pathFive[0] == 0);
    REQUIRE(pathFive[1] == 3);
    REQUIRE(pathFive[2] == 10);
}

TEST_CASE("actual dataset dijkstra's 1", "[dijkstra]") {
    Parser parser;
    std::vector<Airport>* airports = parser.airportsParser("data/datasets/airports.dat.txt"); 
    std::map<std::pair<std::string, std::string>, int>* validRoutes = parser.validRouteParser("data/datasets/routes.dat.txt"); 

    // create a RouteMaker object
    RouteMaker routeMaker = RouteMaker(airports, validRoutes);

    // use Dikstras algorithm
    std::vector<int> vect = routeMaker.Dijkstra(2832, 2811, 6000.0); 

    // make sure the return vector matches the expected outcome
    REQUIRE(vect.size() == 2);
    REQUIRE(vect.at(0) == 2832);
    REQUIRE(vect.at(1) == 2811);
}

TEST_CASE("actual dataset dijkstra's 2", "[dijkstra]") {
    Parser parser;
    std::vector<Airport>* airports = parser.airportsParser("data/datasets/airports.dat.txt"); 
    std::map<std::pair<std::string, std::string>, int>* validRoutes = parser.validRouteParser("data/datasets/routes.dat.txt"); 

    // create a RouteMaker object
    RouteMaker routeMaker = RouteMaker(airports, validRoutes);
 
    // use Dikstras algorithm
    std::vector<int> vect = routeMaker.Dijkstra(0, 28, 5000.0); 

    // make sure the return vector matches the expected outcome
    REQUIRE(vect.size() == 10); 
    REQUIRE(vect.at(0) == 0);
    REQUIRE(vect.at(1) == 4);
    REQUIRE(vect.at(2) == 1894);
    REQUIRE(vect.at(3) == 1901);
    REQUIRE(vect.at(4) == 3528);
    REQUIRE(vect.at(5) == 154);
    REQUIRE(vect.at(6) == 48);
    REQUIRE(vect.at(7) == 194);
    REQUIRE(vect.at(8) == 130);
    REQUIRE(vect.at(9) == 28);
}
 
TEST_CASE("actual dataset dijkstra's with MaxDistance = 0", "[dijkstra]") {
    Parser parser;
    std::vector<Airport>* airports = parser.airportsParser("data/datasets/airports.dat.txt"); 
    std::map<std::pair<std::string, std::string>, int>* validRoutes = parser.validRouteParser("data/datasets/routes.dat.txt"); 

     // create a RouteMaker object
    RouteMaker routeMaker = RouteMaker(airports, validRoutes);
 
    // use Dikstras algorithm
    std::vector<int> vect = routeMaker.Dijkstra(0, 28, 0.0);

    // make sure the return vector has a size of 0, since the Max Distance is 0
    REQUIRE(vect.size() == 0);
}

TEST_CASE("unconnected vertex dijkstra's", "[dijkstra]") {
    //create a custom small graph making sure that the path found is correct
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(6, std::vector<double>(6, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    //setting the edges for that graph
    test.setEdge(0, 1, 2.0);
    test.setEdge(0, 2, 3.0);
    test.setEdge(0, 3, 4.0);
    test.setEdge(1, 3, 3.0);
    // test.setEdge(3, 4, 6.0);
    // test.setEdge(0, 5, 15.0);
    test.setEdge(4, 5, 2.0);

    std::vector<int> pathOne = test.Dijkstra(0, 5, INT_MAX);
    REQUIRE((int)pathOne.size() == 0);

    std::vector<int> pathTwo = test.Dijkstra(0, 4, INT_MAX);
    REQUIRE((int)pathTwo.size() == 0);

}

TEST_CASE("no route airplane range dijkstra's", "[dijkstra]") {
    std::vector<std::vector<double>> testMatrix(std::vector<std::vector<double>>(6, std::vector<double>(6, 0.0)));
    RouteMaker test = RouteMaker();
    test.setMatrix(testMatrix);

    //setting the edges for that graph
    test.setEdge(0, 1, 2.0);
    test.setEdge(0, 2, 3.0);
    test.setEdge(0, 3, 4.0);
    test.setEdge(1, 3, 3.0);
    test.setEdge(3, 4, 6.0);
    test.setEdge(0, 5, 15.0);
    test.setEdge(4, 5, 2.0);

    std::vector<int> pathOne = test.Dijkstra(0, 3, 1);
    REQUIRE((int)pathOne.size() == 0);

    std::vector<int> pathTwo = test.Dijkstra(0, 4, 1);
    REQUIRE((int)pathTwo.size() == 0);

    std::vector<int> pathThree = test.Dijkstra(0, 4, 1);
    REQUIRE((int)pathThree.size() == 0);
}


