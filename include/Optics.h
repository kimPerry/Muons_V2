#ifndef OPTICS_H
#define OPTICS_H

#include "PointPriorityQ.h"

class Optics
{
public:
	static vector<Point>* runOptics(vector<Point>* pointsList, int slice[][MAX_COLS]);
	static void clusters(vector<Point>* orderedList);

private:
	static void update(vector<Point>* genericList, vector<Point>* neighbors, Point point, PointPriorityQ * seeds, int slice[][MAX_COLS]);
    static void getNeighbors(vector<Point>* pointsList, int pointIndex, int slice[][MAX_COLS]);
    static Point* findPointInList(vector<Point>* pointsList, int row, int col);
    static double euclideanDistance(double x1, double x2, double y1, double y2);
};

#endif
