#include <stdlib.h>
#include <Point.h>
#include "Optics.h"
#include <cmath>
#include <iostream>

vector<Point>* Optics::runOptics(vector<Point>* pointsList)
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
			Optics::getNeighbors(pointsList, i);

			// mark point as processed
			pointsList->at(i).processed = true;

			// output p to the ordered list
			orderedList->push_back(pointsList->at(i));

			seeds = new PointPriorityQ;

			// if core distance is defined
			if(pointsList->at(i).coreDistance != UNDEFINEDCD)
			{
				// call update with neighbors
				update(pointsList, i, seeds);

				// for each point in seeds
				while(!seeds->getQueue()->empty())
				{
					// set tempPoint
					tempPoint = seeds->getQueue()->at(0);

					// remove front point
					seeds->getQueue()->erase(seeds->getQueue()->begin());

					// run getNeighbors on that point
					getNeighbors(pointsList, tempPoint.indexInPointsList);

					// set point's core distance
					tempPoint.coreDistance = pointsList->at(tempPoint.indexInPointsList).coreDistance;

					//mark point as processed
					tempPoint.processed = true;
					pointsList->at(tempPoint.indexInPointsList).processed = true;

					// write point to ordered list
					orderedList->push_back(pointsList->at(tempPoint.indexInPointsList));

					if(tempPoint.coreDistance != UNDEFINEDCD)
					{
						update(pointsList, tempPoint.indexInPointsList, seeds);
					}

				}
			}
		}
	}

	return orderedList;
}

void Optics::update(vector<Point>* pointsList, int pointIndex, PointPriorityQ * seeds)
{
	double newReachabilityDist;

	// for each point in neighbors
	for(int i = 0; i < (int)pointsList->at(pointIndex).neighbors->size(); i++)
	{
		// if point is not processes
		if(pointsList->at(pointIndex).neighbors->at(i)->processed == false)
		{
			// set the new reachability distance
			newReachabilityDist = MAX(pointsList->at(pointIndex).coreDistance, pointsList->at(pointIndex).neighbors->at(i)->distanceToP);

			// if the point's reachability distance is undefined
		    if(pointsList->at(pointIndex).neighbors->at(i)->reachabilityDistance == UNDEFINEDRD)
		    {
		    	// set reachability distance
		    	pointsList->at(pointIndex).neighbors->at(i)->reachabilityDistance = newReachabilityDist;

		    	pointsList->at(pointsList->at(pointIndex).neighbors->at(i)->indexInPointsList).reachabilityDistance = newReachabilityDist;

		    	// add point to seeds
		    	seeds->push(*(pointsList->at(pointIndex).neighbors->at(i)));
		    }
		    // if the point's reachability distance is defined
		    else
		    {
		    	// compare the two distances
		    	if(newReachabilityDist < pointsList->at(pointIndex).neighbors->at(i)->reachabilityDistance)
		    	{
		    		// if the new distance is smaller reset this point's distance and move it up in the queue
		    		pointsList->at(pointIndex).neighbors->at(i)->reachabilityDistance = newReachabilityDist;
		    		pointsList->at(pointsList->at(pointIndex).neighbors->at(i)->indexInPointsList).reachabilityDistance = newReachabilityDist;

		    		// move point up in queue
		    		seeds->moveUp(*(pointsList->at(pointIndex).neighbors->at(i)));
		    	}
		    }
		}
	}
}

double Optics::euclideanDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double part1, part2, part3, sum, result;

	part1 = x2 - x1;
	part2 = y2 - y1;
	part3 = z2 - z1;

	sum = part1 * part1 + part2 * part2 + part3 * part3;

	result = sqrt(sum);

	return result;
}

void Optics::getNeighbors(vector<Point>* pointsList, int pointIndex)
{
	vector<Point*> * neighbors = new vector<Point*>;
    Point * tempPoint;
    double distCalc;
    double coreDistance = EPS + 1;

    // for every point
    for(int i = 0; i < (int)pointsList->size(); i++)
    {
    	if(pointIndex != i)
    	{
			distCalc = euclideanDistance(pointsList->at(pointIndex).x, pointsList->at(pointIndex).y, pointsList->at(pointIndex).z, pointsList->at(i).x, pointsList->at(i).y, pointsList->at(i).z);
			//cout << "distance: " << distCalc << endl;
			if(distCalc <= EPS)
			{
				tempPoint = &(pointsList->at(i));

				tempPoint->distanceToP = distCalc;
				neighbors->push_back(tempPoint);
				if(tempPoint->distanceToP < coreDistance)
				{
					coreDistance = tempPoint->distanceToP;
				}
			}
    	}
    }

	if((int)neighbors->size() >= MIN_PTS)
	{
		pointsList->at(pointIndex).coreDistance = coreDistance;
		//cout << "core distance: " << coreDistance << endl;
	}

	pointsList->at(pointIndex).neighbors = neighbors;
}

vector<vector<Point> > Optics::clusters(vector<Point>* orderedList)
{
	int cluster = 0;

	vector<vector<Point> > clusterList;
	vector<Point> tempVector;

	// for each point in the ordered list
	for(int i = 0; i < (int)orderedList->size(); i++)
	{
		if(orderedList->at(i).reachabilityDistance > EPS)
		{
			if(orderedList->at(i).coreDistance <= EPS)
			{
				orderedList->at(i).clusterID = cluster;
				//clusterList.at(cluster) = new vector<Point>;
				tempVector.push_back(orderedList->at(i));
				clusterList.push_back(tempVector);
				cluster++;
				tempVector.clear();
			}
			else
			{
				orderedList->at(i).clusterID = NOISE;
			}
		}
		else
		{
		    if(orderedList->at(i).coreDistance <= EPS)
			{
				  orderedList->at(i).clusterID = cluster;
				  tempVector.push_back(orderedList->at(i));
			}
			else
			{
				orderedList->at(i).clusterID = NOISE;
			}
		}
	}

	return clusterList;
}
