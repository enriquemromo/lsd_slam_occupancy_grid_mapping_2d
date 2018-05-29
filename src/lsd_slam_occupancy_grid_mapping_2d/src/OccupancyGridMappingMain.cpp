#include <stdio.h>
#include "ros/ros.h"
#include "boost/thread.hpp"
#include "std_msgs/String.h"
#include "settings.h"
#include <vector>
#include <fstream>

#include "lsd_slam_occupancy_grid_mapping_2d/PointCloudMsg.h"
#include "lsd_slam_occupancy_grid_mapping_2d/PointCloudArrayMsg.h"
#include <qapplication.h>
#include <QtGui>

#include "OccupancyGridMapping2D.h"
#include "PointCloud.h"




/**
* This tutorial demonstrates simple receipt of messages over the ROS system.
*/

PointCloud* pointCloud;
OccupancyGridMapping2D* occupancyGridMapping2D = 0;

using namespace std;
using namespace ros;

Point changeValue(double x, double z){
	double a = (x - (Xmin)) *  screenSize / (Xmax - (Xmin));
	double b = (z - (Ymax)) * -screenSize / (Ymax - (Ymin));	
	Point tempPoint;
	tempPoint.x = a;
	tempPoint.z = b;
	return tempPoint;
}

void readFile()
{
	std::string delimiter = ",";
	string line;
	int limit = 0;
	ifstream myfile ("/home/toto/laboratorioSLAMFullFormated.ply");
	int index = 0;
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
    		{
			size_t pos = 0;
			std::string token;
			string data[3];
			int index = 0;
				
			//	cout << line<<endl;
			

			while ((pos = line.find(delimiter)) != std::string::npos) 
			{
				token = line.substr(0, pos);
				data[index]=token;
				line.erase(0, pos + delimiter.length());
				index++;
			}
			
			data[index] = line;
	
	
	
			double x = strtof((data[0]).c_str(),0);
			double y = strtof((data[1]).c_str(),0);
			double z = strtof((data[2]).c_str(),0);
			index++;
			
			limit ++;
			
			
			
			if( minY < y && y < maxY ) {

			//ROS_INFO_STREAM( "x " << x << ", y " << y << ", z " << z);
				Point point = changeValue(x, z);
			//ROS_INFO_STREAM( "x " << point.x << ", y " << point.y << ", z " << point.z);
				pointCloud->setSortedPoint(point.x,y,point.z);
			}

 		}
		myfile.close();

	}
}



void poseLSDSlamCallback(const lsd_slam_occupancy_grid_mapping_2d::PointCloudArrayMsg::ConstPtr& msg)
{
	for (int i=0; i<msg->points.size(); ++i)
    {
      const lsd_slam_occupancy_grid_mapping_2d::PointCloudMsg &data = msg->points[i];
      ROS_INFO_STREAM("x: " << data.x << "y " << data.y <<
                      "z " << data.z );
                  if( minY < data.y && data.y < maxY ) {
                      //pointCloud->setPoint(data.x, data.y, data.z);
					  Point point = changeValue(data.x,data.z);
                      pointCloud->setSortedPoint(point.x, data.y, point.z);
				  }
    }
	
}


void createOccupancyGridMapping2D( int argc, char** argv )
{	
		printf("Started ROS thread\n");
		ros::init(argc, argv, "occupy_2d");
		ROS_INFO("lsd_slam_occupancy_grid_mapping_2d started");
		ros::NodeHandle n;

	 	Subscriber liveFrames_sub = n.subscribe(n.resolveName("/lsd_slam/pointcloud"),1, poseLSDSlamCallback);
		ros::spin();
		printf("Exiting ROS thread\n");

	exit(1);
		
}

void loop()
{
	if(occupancyGridMapping2D != 0)
	{
		while(true)
		{
			occupancyGridMapping2D->process();
			boost::this_thread::sleep( boost::posix_time::milliseconds(60000) );
		}
	}
}

int main(int argc, char **argv)
{	
		ROS_INFO("occupancy_grid_mapping_2D started");
		pointCloud = new PointCloud();
		
		if(test){
			ROS_INFO("TEST MODE");
			readFile();
		}
		QApplication app( argc, argv );
		
		occupancyGridMapping2D = new OccupancyGridMapping2D();
		occupancyGridMapping2D->setPoint(pointCloud);
		occupancyGridMapping2D->setWindowTitle("Occupancy Grid Mapping 2D");
		
		occupancyGridMapping2D->show();
		
		if(!test){
			boost::thread rosThread  = boost::thread(createOccupancyGridMapping2D, argc, argv);
		}
		boost::thread t1 = boost::thread(loop);
		
		return app.exec();
}



