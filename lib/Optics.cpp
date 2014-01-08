#include <stdlib.h>
#include <Point.h>
#include "Optics.h"
#include <cmath>

vector<Point>* Optics::runOptics(vector<Point>* pointsList, int slice[][MAX_COLS])
{
	vector<Point>* orderedList = new vector<Point>;
	PointPriorityQ * seeds;
	Point tempPoint;

	// for each point in pointsList
	for(int i = 0; i < (int)pointsList->size(); i++)
	{
		// if the point is unprocessed
		if(pointsList->at(i).processed == false)
		{
			// get neighbors and core distance for that point
			Optics::getNeighbors(pointsList, i, slice);

			// mark point as processed
			pointsList->at(i).processed = true;

			// output p to the ordered list
			orderedList->push_back(pointsList->at(i));

			seeds = new PointPriorityQ;

			// if core distance is defined
			if(pointsList->at(i).coreDistance != UNDEFINEDCD)
			{
				// call update with neighbors
				update(pointsList, pointsList->at(i).neighbors, pointsList->at(i), seeds, slice);

				// for each point in seeds
				while(!seeds->getQueue()->empty())
				{
					// set tempPoint

					tempPoint = seeds->getQueue()->at(0);
					// remove front point
					seeds->getQueue()->erase(seeds->getQueue()->begin());

					// run getNeighbors on that point
					getNeighbors(pointsList, tempPoint.indexInPointsList, slice);

					// set point's core distance
					tempPoint.coreDistance = pointsList->at(tempPoint.indexInPointsList).coreDistance;

					//mark point as processed
					tempPoint.processed = true;
					pointsList->at(tempPoint.indexInPointsList).processed = true;

					// write point to ordered list
					orderedList->push_back(tempPoint);

					if(tempPoint.coreDistance != UNDEFINEDCD)
					{
						update(pointsList, tempPoint.neighbors, tempPoint, seeds, slice);
					}
				}
			}
		}
	}

	return orderedList;
}

void Optics::update(vector<Point>* genericList, vector<Point>* neighbors, Point point, PointPriorityQ * seeds, int slice[][MAX_COLS])
{
	int newReachabilityDist;

	// for each point in neighbors
	for(int i = 0; i < (int)neighbors->size(); i++)
	{
		// if point is not processes
		if(neighbors->at(i).processed == false)
		{
			// set the new reachability distance
			if(point.coreDistance == UNDEFINEDCD)

			newReachabilityDist = MAX(point.coreDistance, neighbors->at(i).distanceToP);

			// if the point's reachability distance is undefined
		    if(neighbors->at(i).reachabilityDistance == UNDEFINEDRD)
		    {
		    	// set reachability distance
		    	neighbors->at(i).reachabilityDistance = newReachabilityDist;
		    	genericList->at(neighbors->at(i).indexInPointsList).reachabilityDistance = newReachabilityDist;

		    	// add point to seeds
		    	seeds->push(neighbors->at(i));

		    }
		    // if the point's reachability distance is defined
		    else
		    {
		    	// compare the two distances
		    	if(newReachabilityDist < neighbors->at(i).reachabilityDistance)
		    	{
		    		// if the new distance is smaller reset this point's distance and move it up in the queue
		    		neighbors->at(i).reachabilityDistance = newReachabilityDist;
		    		genericList->at(neighbors->at(i).indexInPointsList).reachabilityDistance = newReachabilityDist;

		    		// move point up in queue
		    		seeds->moveUp(neighbors->at(i));
		    	}
		    }
		}
	}
}

double Optics::euclideanDistance(double x1, double x2, double y1, double y2)
{
	double p1 = x2 - x1;
	double p2 = y2 - y1;
	double result = sqrt((p1 * p1) + (p2 * p2));

	return result;
}

void Optics::getNeighbors(vector<Point>* pointsList, int pointIndex, int slice[][MAX_COLS])
{
	vector<Point> * neighbors = new vector<Point>;
    Point * tempPoint;
    double minDistance = 1000;
    double distCalc;
    int coreDistance = EPS + 1;

    // for every point
    for(int i = 0; i < pointsList->size(); i++)
    {
    	distCalc = euclideanDistance(pointsList->at(pointIndex).rowPos, pointsList->at(i).rowPos, pointsList->at(pointIndex).colPos, pointsList->at(i).colPos);

    	if(distCalc <= EPS)
    	{

    		tempPoint = &(pointsList->at(i));

			tempPoint->distanceToP = distCalc;
			neighbors->push_back(*tempPoint);
			if(tempPoint->distanceToP < coreDistance)
			{
				coreDistance = tempPoint->distanceToP;
			}
    	}
    }
    /*
    // + x direction
	for(int i = 1; i <= EPS; i++)
	{
		// x + i
		if(pointsList->at(pointIndex).rowPos + i < MAX_ROWS)
		{
			if(slice[pointsList->at(pointIndex).rowPos + i][pointsList->at(pointIndex).colPos] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos + i, pointsList->at(pointIndex).colPos);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// y + i
		if(pointsList->at(pointIndex).colPos + i < MAX_COLS)
		{
			if(slice[pointsList->at(pointIndex).rowPos][pointsList->at(pointIndex).colPos + i] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos, pointsList->at(pointIndex).colPos + i);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// x + i & y + i
		if(pointsList->at(pointIndex).rowPos + i < MAX_ROWS && pointsList->at(pointIndex).colPos + i < MAX_COLS)
		{
			if(slice[pointsList->at(pointIndex).rowPos + i][pointsList->at(pointIndex).colPos + i] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos + i, pointsList->at(pointIndex).colPos + i);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i * i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// x - i & y + i
		if(pointsList->at(pointIndex).rowPos - i >=0 && pointsList->at(pointIndex).colPos + i < MAX_COLS)
		{
			if(slice[pointsList->at(pointIndex).rowPos - i][pointsList->at(pointIndex).colPos + i] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos - i, pointsList->at(pointIndex).colPos + i);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i * i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// x + i & y - i
		if(pointsList->at(pointIndex).rowPos + i < MAX_ROWS && pointsList->at(pointIndex).colPos - i >= 0)
		{
			if(slice[pointsList->at(pointIndex).rowPos + i][pointsList->at(pointIndex).colPos - i] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos + i, pointsList->at(pointIndex).colPos - i);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i * i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// x - i & y - i
		if(pointsList->at(pointIndex).rowPos - i >= 0 && pointsList->at(pointIndex).colPos - i >= 0)
		{
			if(slice[pointsList->at(pointIndex).rowPos - i][pointsList->at(pointIndex).colPos - i] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos - i, pointsList->at(pointIndex).colPos - i);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i * i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// x - i
		if(pointsList->at(pointIndex).rowPos - i >= 0)
		{
			if(slice[pointsList->at(pointIndex).rowPos - i][pointsList->at(pointIndex).colPos] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos - i, pointsList->at(pointIndex).colPos);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i * i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

		// y - i
		if(pointsList->at(pointIndex).colPos - i >= 0)
		{
			if(slice[pointsList->at(pointIndex).rowPos][pointsList->at(pointIndex).colPos - i] == 1)
			{
				tempPoint = findPointInList(pointsList, pointsList->at(pointIndex).rowPos, pointsList->at(pointIndex).colPos - i);

				if(tempPoint != NULL)
				{
					tempPoint->distanceToP = i * i;
					neighbors->push_back(*tempPoint);
					if(tempPoint->distanceToP < coreDistance)
					{
						coreDistance = tempPoint->distanceToP;
					}
				}
			}
		}

	}
*/
	if((int)neighbors->size() >= MIN_PTS)
	{
		pointsList->at(pointIndex).coreDistance = coreDistance;
	}

	pointsList->at(pointIndex).neighbors = neighbors;

	neighbors->clear();
}

Point* Optics::findPointInList(vector<Point>* pointsList, int row, int col)

{
	// find point in list
	for(int k = 0; k < (int)pointsList->size(); k++)
	{
		if(pointsList->at(k).rowPos == row)
		{
			if(pointsList->at(k).colPos == col)
			{
				return &pointsList->at(k);
			}
		}
	}

	return NULL;
}

bool withinDistance(double cRow, double cCol, double tRow, double tCol)
{
	double pt1, pt2, sum;

	pt1 = cRow - tRow;
	pt2 = cCol - tCol;

	sum = pt1 * pt1 + pt2 * pt2;
	if(sqrt(sum) <= EPS)
	{
		return true;
	}
	return false;
}

void Optics::clusters(vector<Point>* orderedList)
{
	int cluster = 0;

	// for each point in the ordered list
	for(int i = 0; i < (int)orderedList->size(); i++)
	{
		if(orderedList->at(i).reachabilityDistance > EPS)
		{
			if(orderedList->at(i).coreDistance <= EPS && orderedList->at(i).coreDistance != UNDEFINEDCD)
			{
				cluster++;
				orderedList->at(i).clusterID = cluster;
			}
			else
			{
				orderedList->at(i).clusterID = NOISE;
			}
		}
		else
		{
			orderedList->at(i).clusterID = cluster;
		}
	}
}
