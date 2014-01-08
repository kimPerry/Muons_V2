#ifndef POINTPRIORITYQ_H
#define POINTPRIORITYQ_H

#include <vector>
#include "Point.h"

using namespace std;

class PointPriorityQ
{
private:
	vector<Point> * queue;

public:
	void push(Point & point);
	Point pop();
	void list();
	void moveUp(Point point);
	vector<Point> * getQueue();

	// constructor
	PointPriorityQ();
	// destructor
	~PointPriorityQ();
};

#endif
