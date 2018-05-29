#include "ros/ros.h"
#include <iostream>
#include <algorithm>
#include "VirtualEnvironment.h"
#include "OccupancyGridMapping2D.h"
#include <queue>


using namespace std; 


bool my_compare(const struct Point &left, const struct Point &right)
	{
		if(left.x == right.x)
		{
			if(left.y == right.y)
			{
				return (left.z < right.z);
			} 
			else 
			{
				return (left.y < right.y);
			}
		}
		return (left.x < right.x);
	}
	bool simple_compare(const struct Point &left, const struct Point &right)
	{
		if(left.x == right.x)
		{
			return (left.z < right.z);
		}
		return (left.x < right.x);
	}
	
OccupancyGridMapping2D::OccupancyGridMapping2D()
{
	maxDistanceVoxel =  sqrt ( pow (voxelSize , 2) * 3 );
	double voxelInRange = (Xmax - Xmin) / voxelSize;

	voxelSizeChanged = screenSize / voxelInRange;
}


void OccupancyGridMapping2D::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	drawGrid(&painter);
	drawMap(&painter);
	drawPoints(&painter);
}


QSize  OccupancyGridMapping2D::sizeHint() const
{
    return QSize(screenSize, screenSize);
}

QSize OccupancyGridMapping2D::minimumSizeHint() const
{
    return QSize(screenSize, screenSize);
}


void OccupancyGridMapping2D::drawGrid(QPainter *painter)
{	
	painter->setPen(Qt::black);
	painter->setBrush(Qt::white);
	for(it = this->voxelsGrid.begin(); it != this->voxelsGrid.end(); ++it )
	{
		QRectF rectangle((*it).x,(*it).y,voxelSizeChanged,voxelSizeChanged);	
   		painter->drawRect(rectangle);
	}
	
}
void OccupancyGridMapping2D::setPoint(PointCloud* pointCloud)
{
	this->pointCloud = pointCloud;
	
}


void OccupancyGridMapping2D::drawMap(QPainter *painter)
{
	meddleMutex.lock();
	painter->setPen(Qt::black);
	painter->setBrush(Qt::red);
	for(it = this->voxels.begin(); it != this->voxels.end(); ++it )
	{
		drawRectagle(painter,(*it).x,(*it).y);
	}
	meddleMutex.unlock();
}

void OccupancyGridMapping2D::drawPoints(QPainter *painter)
{
	meddleMutex.lock();
	for(it = this->voxelPoints.begin(); this->it != voxelPoints.end(); ++it )
	{
		painter->setPen(Qt::blue);
        painter->drawPoint((*it).x, (*it).z);
	}
	meddleMutex.unlock();
}
	
void OccupancyGridMapping2D::drawRectagle(QPainter *painter, double x, double y)
{
	//double a = (x - (Xmin)) *  screenSize / (Xmax - Xmin);
	//double b = (y - (Ymax)) * -screenSize / (Ymax - Ymin);	
	QRectF rectangle(x,y,voxelSizeChanged,voxelSizeChanged);
 	painter->setBrush(Qt::red);
    painter->drawRect(rectangle);
		
}
	


void OccupancyGridMapping2D::process()
{
	this->voxels.clear();
	ROS_INFO("edgeVoxel %f", voxelSizeChanged);
	ROS_INFO("numPoints %d", numPoints);
	ROS_INFO("maxDistanceVoxel %f", maxDistanceVoxel);


	deque<Point>  pointsCloud = this->pointCloud->getSortedPoints();
 

	sort (pointsCloud.begin(), pointsCloud.end(), simple_compare); 
	
	
	std::deque<Point>::iterator iter;
	
	for(iter = pointsCloud.begin(); iter != pointsCloud.end(); ++iter )
	{
		voxelPoints.push_back((*iter));
	}
	
	double tempx;
	double tempy;
	int points;
	double x;
	double y;
	int index = 0;
	Point currentPoint;
	
	
	for(x = 0; x <= 700; x+= voxelSizeChanged)
	{
		tempx = x + voxelSizeChanged;
		for(y = 0; y <= 700; y+= voxelSizeChanged)
		{
			tempy = y + voxelSizeChanged;
				
			//printf("x1 = %f,  x2 = %f \n",x,tempx);
			//printf("y1 = %f,  y2 = %f \n",y,tempy);
			//printf("z1 = %f,  z2 = %f \n",z,tempz);			
			Point quad;
			quad.x = (x+voxelSizeChanged/2);
			quad.y = (y+voxelSizeChanged/2);
			this->voxelsGrid.push_back(quad);
			if(!pointsCloud.empty())
			{
				currentPoint = pointsCloud.at(index);
				
			}	
			else
			{				
				break;
			} 
			while(currentPoint.x < tempx)
			{
				currentPoint = pointsCloud.at(index);
				//printf("Point x = %f,  y = %f, z = %f \n",currentPoint.x,currentPoint.y,currentPoint.z);
				if( x <= currentPoint.x && currentPoint.x  <= tempx  &&
				 y <= currentPoint.z && currentPoint.z <= tempy)
				{
					points++;
					pointsCloud.erase(pointsCloud.begin()+index);
					//printf("Point x = %f,  y = %f, z = %f \n",currentPoint.x,currentPoint.y,currentPoint.z);
				} 
				index++;
				if(index < pointsCloud.size())
				{
					currentPoint = pointsCloud.at(index);
				}
				else
				{
					break;
				}
			}
	
			if( points >= numPoints)
			{				
				Point voxelTemp;
				voxelTemp.x = (x+(voxelSizeChanged/2));
				voxelTemp.y = (y+(voxelSizeChanged/2));
				//voxelTemp.z = (z+(voxelSize/2));
				//printf("Point x = %f,  y = %f, z = %f \n",quad.x,quad.y,quad.z);
				this->voxels.push_back(voxelTemp);
			} 	
			points = 0;
			index = 0;
		}
	}

	int size = this->voxels.size();

	printf("Total Voxels %d \n",size);
	ROS_INFO("Termino");
}

void OccupancyGridMapping2D::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
    {      
		case Qt::Key_G :
			ROS_INFO("Generando Entorno Virtual");
			VirtualEnvironment virtualEnvironment;
			virtualEnvironment.generate(this->voxels);
        break;
    }
}
