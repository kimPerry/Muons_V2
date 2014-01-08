#ifndef POINT_H
#define POINT_H

#include <vector>
#include "Constants.h"

using namespace std;

class Point
{
	public:
	double x;
	double y;
	double z;
	double rowPos;
	double colPos;
	double coreDistance;
	double scatteringAngle;
	double momentum;
	double reachabilityDistance;
	double distanceToP;
	double indexInPointsList;
	bool processed;
	int clusterID;
	vector<Point> * neighbors;
	void operator= (const Point & rightSide);

	// constructor
	Point();
	// copy constructor
	Point(const Point & right);
	// destructor
	~Point();
};

#endif
