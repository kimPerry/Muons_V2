//============================================================================
// Name        : GraphTest.cpp
// Author      : Kim Perry
// Description : OPTICS Clustering
//============================================================================


#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <stdio.h>
#include "Point.h"
#include "PointPriorityQ.h"
#include "Optics.h"

using namespace std;

struct muonTrack
{
	float time;
	float scatAngleTotal;
	float pocaX;
	float pocaY;
	float pocaZ;
};

double fileReadIn(string filename, vector<muonTrack>* muonTracks);
void writeToFile(ofstream & fout, vector<Point> points);

int main() {

	vector<Point> tempList;
	vector<Point>* orderedPoints = new vector<Point>;

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

	bool iterative = false;

	char filename[] = "MTTrackEpoch_852.csv";

	ifstream fin;
	ofstream fout;
	fin.open(filename);

	vector<Point> points;
	Point * point = NULL;

    // stop new lines from being skipped
    fin.unsetf(std::ios_base::skipws);

    // count the newlines
    int lineCount = count(istream_iterator<char>(fin), istream_iterator<char>(),
        '\n');

	fin.close();

	cout << lineCount << " tracks" << endl;

	// vector of muon track data
	vector<muonTrack> * muonTracks = new vector<muonTrack>;

	// read in file
	cout << "Reading file..." << endl;
	fin.open(filename);
	initialTime = fileReadIn(filename, muonTracks) / divisor;
	fin.close();
	cout << "Number of muons over " << SCAT_ANGLE << " : " << muonTracks->size() << endl;

	//fout.open("allPocas.dat");

	initialTime = muonTracks->at(0).time / divisor;
	totalSeconds = 60 * TIME_MINUTES;
	initialTime = round(initialTime);
	endTime = initialTime + totalSeconds;

	cout << "Making a list..." << endl;

	// gather muons in time frame
	for(int i = 0; i < muonTracks->size(); i++)
	//{
	//
	//while((muonTracks->at(tracker).time / divisor) < endTime)
	{
		if(tracker > 0 && (muonTracks->at(tracker).time / divisor) < (muonTracks->at(tracker-1).time / divisor))
		{
			elapsedTime = muonTracks->at(tracker-1).time/divisor - initialTime;
			totalElapsedTime += elapsedTime;
			endTime = (muonTracks->at(tracker).time/divisor) + (totalSeconds - totalElapsedTime);
		}

		// if track has valid POCA values
		if(muonTracks->at(tracker).pocaX > 0 && muonTracks->at(tracker).pocaY > 0 && muonTracks->at(tracker).pocaZ > 0)
		{
			// if track has scattering angle over a certain threshold
			if(muonTracks->at(tracker).scatAngleTotal > SCAT_ANGLE)
			{
				point = new Point;
				point->x = muonTracks->at(tracker).pocaX;
				point->y = muonTracks->at(tracker).pocaY;
				point->z = muonTracks->at(tracker).pocaZ;
				point->scatteringAngle = muonTracks->at(tracker).scatAngleTotal;
				point->indexInPointsList = indexPL;
				points.push_back(*point);
				indexPL++;
			}
		}

		tracker++;

	}

	cout << "end loop" << endl;

	fout.open("allPocas.dat");
	// write out all POCAs for comparison
	writeToFile(fout, points);
    fout.close();

	cout << "Beginning OPTICS..." << endl;
	// call optics
	orderedPoints = Optics::runOptics(&points);

	cout << "Finding clusters..." << endl;
	vector<vector<Point> > clusterList;
	clusterList = Optics::clusters(orderedPoints);
	indexPL = 0;


	// read out to file
	cout << "Writing file..." << endl;
	fout.open("optics.dat");

	int numPoints = 0;
	double xmin = 10000, ymin = 10000, zmin = 10000, xmax = 0, ymax = 0, zmax = 0;

	for(int i = 0; i < clusterList.size(); i++)
	{
		for(int j = 0; j < clusterList.at(i).size(); j++)
		{
			if(clusterList.at(i).size() >= MIN_PTS)
			{

			if(clusterList.at(i).at(j).x > 20){

				fout << clusterList.at(i).at(j).x << "\t" << clusterList.at(i).at(j).y
										<< "\t" << clusterList.at(i).at(j).z << endl;
				numPoints++;
				}
			}
		}
	}

	fout.close();

	cout << "high scattering points after checking: " << numPoints << endl;

	orderedPoints->clear();
	delete muonTracks;
	delete point;
	return 0;
}

void writeToFile(ofstream & fout, vector<Point> points)
{
	for(int i = 0; i < (int)points.size(); i++)
	{
		fout << points[i].x << "\t" << points[i].y << "\t" << points[i].z << "\t" << points[i].scatteringAngle * 70 << endl;
	}
}

// big data files
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

double fileReadIn(string filename, vector<muonTrack>* muonTracks)
{
	string dummy;
	double startTime, time, scatAngleTotal;
	muonTrack * temp;
	int index = 0;

	cout << "test" << endl;
    ifstream fin;
	fin.open(filename);


	cout << filename << endl;
	if(fin.good())
	{
		cout << "googd" << endl;
	}

	while(getline(fin, dummy, ',' ))
	{
		time = atof(dummy.c_str());
		if(index == 0)
		{
			startTime = time;
		}

		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );

		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );

		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );

		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );

		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );

		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );
		getline(fin, dummy, ',' );

		scatAngleTotal = atof(dummy.c_str());
		if(scatAngleTotal > SCAT_ANGLE)
		{
			temp = new muonTrack;
			temp->time = time;
			temp->scatAngleTotal = scatAngleTotal;
			// POCA x y and z
			getline(fin, dummy, ',' );
			temp->pocaX = atof(dummy.c_str());
			getline(fin, dummy, ',' );
			temp->pocaY = atof(dummy.c_str());
			getline(fin, dummy, ',' );
			temp->pocaZ = atof(dummy.c_str());
			muonTracks->push_back(*temp);
		}
		else
		{
			// POCA x y and z
			getline(fin, dummy, ',' );
			getline(fin, dummy, ',' );
			getline(fin, dummy, ',' );
		}
		index++;
	}
	fin.close();
	return startTime;
}
