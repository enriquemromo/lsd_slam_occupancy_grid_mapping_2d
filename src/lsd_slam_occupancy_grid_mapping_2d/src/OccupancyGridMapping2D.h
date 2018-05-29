#include <QtGui>
#include <QWidget>
#include <QRectF>
#include "PointCloud.h"
#include "settings.h"
#include "boost/thread.hpp"

class OccupancyGridMapping2D : public QWidget 
{
	public:
		OccupancyGridMapping2D();
		void paintEvent(QPaintEvent *event);
		void setPoint(PointCloud* pointCloud);
		QSize minimumSizeHint() const override;
		QSize sizeHint() const override;
		void keyPressEvent(QKeyEvent *e);
		void process();
		
	private:
		PointCloud* pointCloud;
		boost::mutex meddleMutex;
		std::vector<Point> voxels;
		std::vector<Point> voxelsGrid;
		std::vector<Point> voxelPoints;
		std::vector<Point>::iterator it;
		float maxDistanceVoxel;
		double voxelSizeChanged;
		void draw(QPainter *painter);
		void drawGrid(QPainter *painter);
		void drawPoints(QPainter *painter);
		//void drawPixel(QPainter *painter);
		void drawMap(QPainter *painter);
		void drawRectagle(QPainter *painter, double x, double y);		
};
