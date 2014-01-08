#include "PointPriorityQ.h"
#include "Point.h"
#include <vector>
#include <iostream>


PointPriorityQ::PointPriorityQ()
{
	queue = new vector<Point>;
}

PointPriorityQ::~PointPriorityQ()
{
	queue->clear();
}

void PointPriorityQ::push(Point & point)
{
	int i = 0;
	if(queue->size() > 0)
	{

		for(i = 0; i < (int)queue->size(); i++)
		{
			if(point.reachabilityDistance < queue->at(i).reachabilityDistance)
			{
				// insert @ i
				queue->insert(queue->begin() + i, point);
				break;
			}
		}
		if(i == (int)queue->size())
		{
			queue->insert(queue->begin() + queue->size(), point);
		}
	}
	else
	{
		queue->insert(queue->begin(), point);
	}

}

void PointPriorityQ::moveUp(Point point)
{
	if(queue->empty())
	{
		push(point);
	}
	else
	{
		for(int i = 0; i < (int)queue->size(); i++)
		{
			if(queue->at(i).indexInPointsList == point.indexInPointsList)
			{
				queue->erase(queue->begin() + i);
				push(point);
				break;
			}
		}
	}
}

Point PointPriorityQ::pop()
{
	Point back = queue->back();
	queue->pop_back();

	return back;
}

vector<Point>* PointPriorityQ::getQueue()
{
	return queue;
}

void PointPriorityQ::list()
{
	if(queue->size() != 0)
	{
		cout << "pq: ";
		for(int i = 0; i < (int)queue->size(); i++)
		{
			cout << queue->at(i).reachabilityDistance << " ";
		}
		cout << endl;
	}
}
