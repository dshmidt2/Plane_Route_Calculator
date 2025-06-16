#include <utility>
#include <cstdlib>
#include "cs225/PNG.h"

int intPart(float x){
    return (int)x;
}

float floatPart(float x){
    return x - intPart(x);
}


/**
 * @brief helper function that draws a single line according to the start and end coordinates
 * 
 * @param height the height of the image
 * @param start_x the starting x coordinate
 * @param start_y the starting y coordinate
 * @param end_x the ending x coordinate
 * @param end_y the ending y coordinate
 * @param png_ a pointer to the PNG object to be modified
 */
void drawLineHelper(int height, int start_x, int start_y, int end_x, int end_y, cs225::PNG* png_){
    bool switchCord = false;
    // decide if we should change coordinates
    if (std::abs(end_y - start_y) > std::abs(end_x - start_x))
        switchCord = true;

    if (switchCord){
        std::swap(start_x, start_y);
        std::swap(end_x, end_y);
    }
    if (start_x > end_x){
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }

    // calculate the difference between the end coordinate and the start coordinate
    float dx = end_x - start_x;
    float dy = end_y - start_y;
    float curr_y = start_y;
    float gradient;
    if (dx == 0.0)
        gradient = 1;
    else
        gradient = dy/dx;

    int hue = 0;
    // assign hue, saturation, and luminance values to each pixel
    if (switchCord){
        for (int x = start_x; x <= end_x; x++){
            png_->getPixel(intPart(curr_y), height - x).h = hue;
            png_->getPixel(intPart(curr_y) - 1, height - x).h = hue;
            
            png_->getPixel(intPart(curr_y), height - x).s = 1;
            png_->getPixel(intPart(curr_y) - 1, height - x).s = 1;

            png_->getPixel(intPart(curr_y), height - x).l = 0.5;
            png_->getPixel(intPart(curr_y) - 1, height - x).l = 0.5;

            curr_y += gradient;
        } 
    } else {
        for (int x = start_x; x <= end_x; x++){
            png_->getPixel(x, height - intPart(curr_y)).h = hue;
            png_->getPixel(x, height - intPart(curr_y) - 1).h = hue;
            
            png_->getPixel(x, height - intPart(curr_y)).s = 1;
            png_->getPixel(x, height - intPart(curr_y) - 1).s = 1;

            png_->getPixel(x, height - intPart(curr_y)).l = 0.5;
            png_->getPixel(x, height - intPart(curr_y) - 1).l = 0.5;

            curr_y += gradient;
        }
    }

}


/**
 * @brief draw lines that represent flight routes on a world map picture
 * 
 * @param airports a vector of integer that represent the airport indices
 * @return a png world map that has routes drawn on it
 */
cs225::PNG* drawLine(std::vector<int> airports, std::vector<Airport>* airportsData){

    if (airports.size() == 0) {
        std::cout << "there is no valid route hence no image drawn" << std::endl;
        return NULL;
    }

    cs225::PNG* png_ = new cs225::PNG();
    png_->readFromFile("world_map.png");
    int width = png_->width();
    int height = png_->height();
    int null_x = 1972;      // measured on map
    int null_y = 1071;      // measured on map
    float convertRatio = 2080.0 / 180.0;     // ratio to convert from longitude or latitude to pixel
    
    for (unsigned a = 0; a < airports.size() - 1; a++){
        float sourceLong = (*airportsData)[airports[a]].getLong();
        float sourceLat = (*airportsData)[airports[a]].getLat();
        float destLong = (*airportsData)[airports[a+1]].getLong();
        float destLat = (*airportsData)[airports[a+1]].getLat();

        // convert the world coordinates to coordinates of the image
        int start_x, start_y, end_x, end_y;
        start_x = null_x + convertRatio * sourceLong;
        start_y = null_y + convertRatio * sourceLat;
        end_x = null_x + convertRatio * destLong;
        end_y = null_y + convertRatio * destLat;

        // we want to check the coordinate on the left
        if (start_x > end_x){
            std::swap(start_x, end_x);
            std::swap(start_y, end_y);
        }
        int firstLen = (end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y);
        int secondLen = (31-start_x)*(31-start_x) + ((start_y + end_y)/2 - start_y)*((start_y + end_y)/2 - start_y) 
                + (end_x-(width-31))*(end_x-(width-31)) + (end_y-(start_y + end_y)/2)*(end_y-(start_y + end_y)/2);

        // check if the line should be wrapped around the picture
        if (firstLen > secondLen){
            drawLineHelper(height, start_x, start_y, 31, (start_y + end_y)/2, png_);
            drawLineHelper(height, width-31, (start_y + end_y)/2, end_x, end_y, png_);
            continue;
        }

        drawLineHelper(height, start_x, start_y, end_x, end_y, png_);

    }

    return png_;
}

