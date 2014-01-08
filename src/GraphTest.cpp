//============================================================================
// Name        : GraphTest.cpp
// Author      : Kim Perry
// Description : OPTICS Clustering V1
//============================================================================

// TODO
// find out if dynamic vectors are ok
// brute force algo
// non poca test
// mesh plots

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include "Point.h"
#include "PointPriorityQ.h"
#include "Optics.h"

using namespace std;

struct muonTrack
{
	double time;
	double scatAngleTotal;
	double momentum;
	double pocaX;
	double pocaY;
	double pocaZ;
};

void fileReadIn(ifstream & fin, int lineCount, muonTrack * muonTracks );
void clearSlice(int slice[][MAX_COLS]);
void writeToFile(ofstream & fout, vector<Point> points);

int main() {

	vector<Point> tempList;
	vector<vector<Point>* > orderedPoints;
	vector<Point>* orderedListX = new vector<Point>;
	vector<Point>* orderedListY = new vector<Point>;
	vector<Point>* orderedListZ = new vector<Point>;

	int indexPL = 0;

	// timing
	typedef std::numeric_limits< double > dbl;
	cout.precision(dbl::digits10);
	int divisor = 1000000000;
	int tracker = 0;
    double elapsedTime;
    double totalElapsedTime = 0;
	double initialTime;
	double totalSeconds;
	double endTime;

	char filename[] = "MTTrackEpoch_202692.csv";

	ifstream fin;
	ofstream fout;
	fin.open(filename);

	vector<Point> points;
	Point * point = NULL;

	int slice[MAX_ROWS][MAX_COLS];

    // stop new lines from being skipped
    fin.unsetf(std::ios_base::skipws);

    // count the newlines
    int lineCount = count(istream_iterator<char>(fin), istream_iterator<char>(),
        '\n');

	fin.close();

	cout << lineCount << " tracks" << endl;

	// array of muon track data
	muonTrack * muonTracks = new muonTrack[lineCount];

	vector<int>::iterator it;

	// read in file
	cout << "Reading file..." << endl;
	fin.open(filename);
	fileReadIn(fin, lineCount, muonTracks);
	fin.close();
	fout.open("allPocas.dat");

	initialTime = muonTracks[0].time / divisor;
	totalSeconds = 60 * TIME_MINUTES;
	initialTime = round(initialTime);
	endTime = initialTime + totalSeconds;

	cout << "Beginning optics..." << endl;

	// gather muons in time frame
	while((muonTracks[tracker].time / divisor) < endTime)
	{
		if(tracker > 0 && (muonTracks[tracker].time / divisor) < (muonTracks[tracker-1].time / divisor))
		{
			elapsedTime = muonTracks[tracker-1].time/divisor - initialTime;
			totalElapsedTime += elapsedTime;
			endTime = (muonTracks[tracker].time/divisor) + (totalSeconds - totalElapsedTime);
		}

		// if track has valid POCA values
		if(muonTracks[tracker].pocaX > 0 && muonTracks[tracker].pocaY > 0 && muonTracks[tracker].pocaZ > 0)
		{
			// if track has scattering angle over a certain threshold
			if(muonTracks[tracker].scatAngleTotal > SCAT_ANGLE)
			{
				point = new Point;
				point->x = muonTracks[tracker].pocaX;
				point->y = muonTracks[tracker].pocaY;
				point->z = muonTracks[tracker].pocaZ;
				point->scatteringAngle = muonTracks[tracker].scatAngleTotal;
				point->momentum = muonTracks[tracker].momentum;
				points.push_back(*point);
			}
		}
		tracker++;
	}

	// write out all POCAs for comparison
	writeToFile(fout, points);
    fout.close();
	// optics time
	// for every x plane
	for(int i = 0; i < 160; i++)
	{
		clearSlice(slice);
		for(int j = 0; j < (int)points.size(); j++)
		{
			if((int)points[j].x == i)
			{
				if((int)points[j].z < MAX_ROWS && (int)points[j].y < MAX_COLS && (int)points[j].z >=0 && (int)points[j].y >= 0)
				{
					slice[(int)points[j].z][(int)points[j].y] = 1;
					points[j].rowPos = points[j].z;
					points[j].colPos = points[j].y;
					points[j].indexInPointsList = indexPL;
					indexPL++;
					tempList.push_back(points[j]);
				}
			}
		}

		// call optics
		orderedListX = Optics::runOptics(&tempList, slice);
		Optics::clusters(orderedListX);
		tempList.clear();
		indexPL = 0;
		orderedPoints.push_back(orderedListX);
	}

	// for every y plane
	for(int i = 0; i < 160; i++)
	{
		clearSlice(slice);
		for(int j = 0; j < (int)points.size(); j++)
		{
			if((int)points[j].y == i)
			{
				if((int)points[j].z < MAX_ROWS && (int)points[j].x < MAX_COLS && (int)points[j].z >=0 && (int)points[j].x >= 0)
				{
					slice[(int)points[j].z][(int)points[j].x] = 1;
					points[j].rowPos = points[j].z;
					points[j].colPos = points[j].x;
					points[j].indexInPointsList = indexPL;
					indexPL++;
					tempList.push_back(points[j]);
				}

			}
		}

		// call optics
		orderedListY = Optics::runOptics(&tempList, slice);
		Optics::clusters(orderedListY);
		tempList.clear();
		indexPL = 0;
		orderedPoints.push_back(orderedListY);
	}

	// for every z plane
	for(int i = 0; i < 125; i++)
	{
		clearSlice(slice);
		for(int j = 0; j < (int)points.size(); j++)
		{
			if((int)points[j].z == i)
			{
				if((int)points[j].x < MAX_ROWS && (int)points[j].y < MAX_COLS && (int)points[j].x >=0 && (int)points[j].y >= 0)
				{
					slice[(int)points[j].x][(int)points[j].y] = 1;
					points[j].rowPos = points[j].x;
					points[j].colPos = points[j].y;
					points[j].indexInPointsList = indexPL;
					indexPL++;
					tempList.push_back(points[j]);
				}
			}
		}

		// call optics
		orderedListZ = Optics::runOptics(&tempList, slice);
		Optics::clusters(orderedListZ);
		tempList.clear();
		indexPL = 0;
		orderedPoints.push_back(orderedListZ);
	}

	// read out to file
	cout << "Writing file..." << endl;
	fout.open("optics.dat");

	int numPoints = 0;
	for(int i = 0; i < int(orderedPoints.size()); i++)
	{
		for(int j = 0; j < (int)orderedPoints.at(i)->size(); j++)
		{
			if(orderedPoints.at(i)->at(j).clusterID != NOISE)
			{
				fout << orderedPoints.at(i)->at(j).x << "\t" << orderedPoints.at(i)->at(j).y << "\t" << orderedPoints.at(i)->at(j).z << "\t" << orderedPoints.at(i)->at(j).scatteringAngle * 20 << endl;
				numPoints++;
			}
		}
	}
	fout.close();

	cout << "high scattering points after checking: " << numPoints << endl;

	orderedListX->clear();
	orderedListY->clear();
	orderedListZ->clear();
	delete muonTracks;
	delete point;
	return 0;
}

void writeToFile(ofstream & fout, vector<Point> points)
{
	for(int i = 0; i < points.size(); i++)
	{
		fout << points[i].x << "\t" << points[i].y << "\t" << points[i].z << "\t" << points[i].scatteringAngle * 70 << endl;
	}
}

void clearSlice(int slice[][MAX_COLS])
{
	for(int i = 0; i < MAX_ROWS; i++)
	{
		for(int j = 0; j < MAX_COLS; j++)
		{
			slice[i][j] = -1;
		}
	}
}

void fileReadIn(ifstream & fin, int lineCount, muonTrack * muonTracks)
{
	string dummy;

	for(int index = 0; index < lineCount; index++)
	{
		getline(fin, dummy, ',' );
		muonTracks[index].time = atof(dummy.c_str());

		getline(fin, dummy, ',' );
		//muonTracks[index].inDirX = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].inDirY = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].inDirZ = atof(dummy.c_str());

		getline(fin, dummy, ',' );
		//muonTracks[index].inPointX = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].inPointY = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].inPointZ = atof(dummy.c_str());

		getline(fin, dummy, ',' );
		//muonTracks[index].outDirX = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].outDirY = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].outDirZ = atof(dummy.c_str());

		getline(fin, dummy, ',' );
		//muonTracks[index].outPointX = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].outPointY = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].outPointZ = atof(dummy.c_str());

		getline(fin, dummy, ',' );
		muonTracks[index].momentum = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].momentumOut = atof(dummy.c_str());

		getline(fin, dummy, ',' );
		//muonTracks[index].scatAngleX = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		//muonTracks[index].scatAngleY = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		muonTracks[index].scatAngleTotal = atof(dummy.c_str());

		// POCA x y and z
		getline(fin, dummy, ',' );
		muonTracks[index].pocaX = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		muonTracks[index].pocaY = atof(dummy.c_str());
		getline(fin, dummy, ',' );
		muonTracks[index].pocaZ = atof(dummy.c_str());
	}
}
