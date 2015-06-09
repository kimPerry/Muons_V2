#ifndef OPTICS_H
#define OPTICS_H

#include "PointPriorityQ.h"

class Optics
{
public:
	static vector<Point>* runOptics(vector<Point>* pointsList);
	static vector<vector<Point> > clusters(vector<Point>* orderedList);

private:
	static void update(vector<Point>* pointsList, int pointIndex, PointPriorityQ * seeds);
    static void getNeighbors(vector<Point>* pointsList, int pointIndex);
    static double euclideanDistance(double x1, double y1, double z1, double x2, double y2, double z2);
};

#endif
