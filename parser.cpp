#include "parser.h"
 
 
/**
 * airportsParser()
 *
 * @brief go through the dataset extract name,
 * OAI (Openflights Airport Identifier), longitude, latitude. Creates
 * an Airport object and push into returnVect
 *
 * @return a pointer to the vector storing the Airport objects
 */
std::vector<Airport>* Parser::airportsParser(const std::string& filename) {
    std::vector<Airport>* returnVect = new std::vector<Airport>;
    std::fstream file;
 
    file.open(filename, std::ios::in);
 
    if (file.is_open()){
        std::string line;
        while (getline(file, line)){    // store each line into line
            std::stringstream lineStream(line);
            std::string cell, cell2;
            std::string OAI, name;
            double latitude, longitude;
            bool add = true;
            int count = 0;
            while (std::getline(lineStream, cell, ',')){    // store the string until the next comma into cell
                if (count == 0) OAI = cell;
                else if (count == 1){
                    if (cell[cell.length()-1] != '"'){      // handle cases when there is a comma in the airport name
                        std::getline(lineStream, cell2, ',');
                        cell.append(",");
                        cell.append(cell2);
                    }
                    name = cell.substr(1, cell.length()-2);
                }
                else if (count == 2){
                    if (cell[cell.length()-1] != '"'){      // handle cases when there is a comma in the city name
                        std::getline(lineStream, cell2, ',');
                    }
                }  
                // obtain latitude and convert to double; check if entry is valid
                else if (count == 6){
                    try{
                        latitude = std::stod(cell);
                    }
                    catch (std::invalid_argument& e){
                        add = false;
                    }
                }  
                // obtain longitude and convert to double; check if entry is valid
                else if (count == 7){
                    try{
                        longitude = std::stod(cell);
                    }
                    catch (std::invalid_argument& e){
                        add = false;
                    }
                }
                count++;
            }
           
            if (add){
                Airport* entry = new Airport(longitude, latitude, name, OAI);
                returnVect->push_back(*entry);
            }
        }
        file.close();
    }
 
    //return the vect when its done
    return returnVect;
}
 
 
/**
 * validRouteParser()
 *
 * @brief a helper parser that stores valid routes in a map; can be used to check
 * whether a route is valid
 *
 * @return a pointer to the valid routes map
 */
std::map<std::pair<std::string, std::string>, int>* Parser::validRouteParser(const std::string& filename) {
    std::map<std::pair<std::string, std::string>, int>* returnMap = new std::map<std::pair<std::string, std::string>, int>;
    std::fstream file;
 
    file.open(filename, std::ios::in);
 
    if (file.is_open()){
        std::string line;
        while (std::getline(file, line)){    // store each line into line string
            std::stringstream lineStream(line);
            std::string cell;
            std::string source_OAI, dest_OAI;
            int count = 0;
            bool add = true;
            while (std::getline(lineStream, cell, ',')){    // store the string until the next comma into cell
                if (count == 3) source_OAI = cell;
                else if (count == 5) dest_OAI = cell;
                count++;
            }
            try{
                std::stod(source_OAI);
                std::stod(dest_OAI);
            }
            catch (std::invalid_argument& e){
                add = false;
            }
            if (source_OAI != "\\N" && dest_OAI != "\\N" && add){    // check if the source and destination OAI are valid
                std::pair<std::string, std::string> airport_pair = std::pair<std::string, std::string>(source_OAI, dest_OAI);
                returnMap->insert(std::pair<std::pair<std::string, std::string>, int>(airport_pair, 1));
            }
        }
        file.close();
    }
   
    //return the map
    return returnMap;
}
 
 
/**
 * routeParser()
 *
 * @brief parses the routes dataset and stores source_OAI, dest _OAI, stops and
 * equiptment(s).
 *
 * @return a pointer to the 2d vector which stores the data
 */
std::vector<std::vector<std::string> >* Parser::routeParser(const std::string& filename){
    std::vector<std::vector<std::string> >* routes = new std::vector<std::vector<std::string> >;
    std::fstream file;
   
    file.open(filename, std::ios::in);  
 
    if (file.is_open()){
        std::string line;
        while (getline(file, line)){    // store each line into line string
            std::stringstream lineStream(line);
            std::string cell;
            std::string source_OAI, dest_OAI, stops, equipment;
            int count = 0;
            bool add = true;
            while (getline(lineStream, cell, ',')){    // store the string until the next comma into cell
                if (count == 3) source_OAI = cell;
                else if (count == 5) dest_OAI = cell;
                else if (count == 7) stops = cell;
                else if (count == 8) equipment = cell;
                count++;
            }
            try{
                std::stod(source_OAI);
                std::stod(dest_OAI);
            }
            catch (std::invalid_argument& e){
                add = false;
            }
            if (source_OAI != "\\N" && dest_OAI != "\\N" && add){    // check if the source and destination OAI are valid
                std::vector<std::string> v;
                v.push_back(source_OAI);
                v.push_back(dest_OAI);
                v.push_back(stops);
                std::istringstream ss(equipment);
                std::string equi;
                while (ss >> equi){
                    v.push_back(equi);
                }
                routes->push_back(v);
            }
        }
        file.close();
    }
 
    return routes;
}
 
 
/*
parse planes.dat
each line: parse Plane Name and IATA code
create std::vector <Plane>
add to vector new Plane object with name IATA code and set max distance
loop until done with all planes
*/
std::vector<Plane>* Parser::planeParser(const std::string& filename) {

    // Create new Plane List vector
    std::vector <Plane>* plane_list = new std::vector<Plane>;
    
    // Read in the given file
    std::fstream myFile;
    myFile.open(filename, std::ios::in);
 
    if (myFile.is_open()) {
        // Declare File Line
        std::string line;

        // Declare Variables
        std::string planeName;
        std::string planeIATA;
       
        while (getline(myFile, line)) {
 
            // Go through line for first and second comma locations
            size_t firstComma = line.find(",", 0);
            size_t secondComma = line.find(",", firstComma + 1);
   
                // Set variables based on file content
                planeName = line.substr(0, firstComma);
                planeIATA = line.substr(firstComma + 1, secondComma-firstComma-1);

                // Make sure the IATA code is in the proper format
                int countN = 0;

                if (planeIATA != "\\N") {
                    for (unsigned int t = 0; t < (unsigned int)planeIATA.length(); ++t) {
                        if (planeIATA.substr(t, 1) == "\"") {
                            ++countN;
                        }        
                    }
                }

                if (countN != 2) {
                        planeIATA = "INVALID";
                }
 
                // create new Plane using data
                Plane *currPlane = new Plane(planeName, planeIATA);
 
                // add Plane to vector
                plane_list->push_back(*currPlane);    
         }
 
        myFile.close();
     }
 
    // Return Plane List vector
    return plane_list;
 }
 

