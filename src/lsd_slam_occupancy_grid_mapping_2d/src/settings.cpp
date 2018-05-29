/**
* This file is part of LSD-SLAM.
*
* Copyright 2013 Jakob Engel <engelj at in dot tum dot de> (Technical University of Munich)
* For more information see <http://vision.in.tum.de/lsdslam> 
*
* LSD-SLAM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LSD-SLAM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with dvo. If not, see <http://www.gnu.org/licenses/>.
*/

#include "settings.h"

double voxelSize = 0.1;

int Xmin = -2;
int Xmax = 2;
int Ymin = -2;
int Ymax = 2;
int screenSize = 700;
int numPoints = 20;

double minY = 0; //Delimitador minimo  en Y
double maxY = 0.5; // Delimitador maximo en Y

//bool test = false;
bool test = true;
std::string testFile = "/home/toto/LSD_room_pcFormated.ply";




