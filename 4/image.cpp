//---------------------------------------
// Program: image.cpp
// Purpose: Read and print pixels values.
// Author:  John Gauch
// Date:    February 2020
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"

#include <fstream>



//---------------------------------------
// Read color and depth images
//---------------------------------------
void read_images(string color_name, im_color & color_image,
                 string depth_name, im_float & depth_image)
{
   // Read color image
   printf("Reading image %s\n", color_name.c_str());
   color_image.ReadJpg(color_name.c_str());

   // Get image dimensions
   int xdim = color_image.R.Xdim;
   int ydim = color_image.R.Ydim;

   ofstream colorData("color-data.txt");
   // Print color values
   for (int y = 0; y < ydim; y = y + 50)
      for (int x = 0; x < xdim; x = x + 50)
      {
         char buffer[50];
         sprintf(buffer, "%3d %3d %3d %3d %3d", x, y,
            color_image.R.Data2D[y][x],
            color_image.B.Data2D[y][x],
            color_image.G.Data2D[y][x]);
         colorData << buffer << std::endl;
      }

   colorData.close();

   // Read depth image
   printf("Reading image %s\n", depth_name.c_str());
   depth_image.ReadJpg(depth_name.c_str());

   ofstream depthData("depth-data.txt");
   // Get image dimensions
   xdim = depth_image.Xdim;
   ydim = depth_image.Ydim;

   // Print depth values
   for (int y = 0; y < ydim; y = y + 10)
      for (int x = 0; x < xdim; x = x + 10)
      {
         char buffer[50];
         sprintf(buffer, "%d %d %d", x, y, (int)depth_image.Data2D[y][x]);
         depthData << buffer << std::endl;
      }
   depthData.close();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   im_color color;
   im_float depth;
   read_images("penny-image.jpg", color,
               "penny-depth.jpg", depth);
   return 0;
}
