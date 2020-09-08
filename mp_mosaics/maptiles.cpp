/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
#include <vector>
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

     //other variables needed
     vector<Point<3>> points; //store all points
     map<Point<3>, TileImage*> createdMap; //map with all TileImages

     //for each region, take TileImage with closest average color, and push into mCanvas
     typename std::vector<TileImage>::iterator it;
     for(it = theTiles.begin(); it != theTiles.end(); it++){
     //for(size_t t = 0; t < theTiles.size(); t++){
       LUVAPixel curr = it->getAverageColor();
       //convert to Point<3>
       Point<3> curr_point = convertToXYZ(curr);
       //push to points list and map
       points.push_back(curr_point);
       //push the TileImage into the location
       createdMap[curr_point] = &*it;
     }
     //make kdtree
     KDTree<3>* canvas_ = new KDTree<3>(points);
     //create a dynamically allocated MosaicCanvas, with same number of rows and columns as sourceImage, return pointer
     MosaicCanvas* mCanvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
     //each tile in mCanvas set to appropriate TileImage using findNearestNeighbor
     for(int i = 0; i < theSource.getRows(); i++){
       for(int k = 0; k < theSource.getColumns(); k++){
         //get region color, then compare in kdtree
         LUVAPixel region = theSource.getRegionColor(i, k);
         Point<3> regioncolor = convertToXYZ(region);
         Point<3> compare = canvas_->findNearestNeighbor(regioncolor);
         //pull from map the best tile image, then push it onto the mCanvas
         TileImage* closest = createdMap[compare];
         mCanvas->setTile(i, k, closest);
       }
     }
     delete canvas_;
     canvas_ = NULL;
    return mCanvas;


}
