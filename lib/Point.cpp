#include "Point.h"
#include <vector>

// Point constructor
Point::Point()
{
	x = 0;
	y = 0;
	z = 0;
	rowPos = 0;
	colPos = 0;
	scatteringAngle = 0;
	momentum = 0;
	distanceToP = 0;
	processed = false;
	reachabilityDistance = UNDEFINEDRD;
	clusterID = NOISE;
	coreDistance = UNDEFINEDCD;
    neighbors = new vector<Point>;
    indexInPointsList = 0;
}

Point::Point(const Point & rightSide)
{
	x = rightSide.x;
	y = rightSide.y;
	z = rightSide.z;
	rowPos = rightSide.rowPos;
	colPos = rightSide.colPos;
	coreDistance = rightSide.coreDistance;
	scatteringAngle = rightSide.scatteringAngle;
	indexInPointsList = rightSide.indexInPointsList;
	momentum = rightSide.momentum;
	reachabilityDistance = rightSide.reachabilityDistance;
	distanceToP = rightSide.distanceToP;
	processed = rightSide.processed;
	neighbors = rightSide.neighbors;
	clusterID = rightSide.clusterID;
}

// Point destructor
Point::~Point()
{
	neighbors->clear();
}

void Point:: operator= (const Point & rightSide)
{
	x = rightSide.x;
	y = rightSide.y;
	z = rightSide.z;
	coreDistance = rightSide.coreDistance;
	scatteringAngle = rightSide.scatteringAngle;
	indexInPointsList = rightSide.indexInPointsList;
	momentum = rightSide.momentum;
	reachabilityDistance = rightSide.reachabilityDistance;
	distanceToP = rightSide.distanceToP;
	processed = rightSide.processed;
	neighbors = rightSide.neighbors;
	clusterID = rightSide.clusterID;
}
