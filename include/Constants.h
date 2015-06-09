#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
int const MAX_ROWS = 120;
int const MAX_COLS = 160;
int const MIN_PTS = 5;
double const EPS = 1.35;
int const UNDEFINEDRD = 50;
int const UNDEFINEDCD = 50;
int const NOISE = -1;
int const TIME_MINUTES = 600;
double const SCAT_ANGLE = .175;
*/
int const MAX_ROWS = 120;
int const MAX_COLS = 160;
int const MIN_PTS = 5;
double const EPS = 2;
int const UNDEFINEDRD = 50;
int const UNDEFINEDCD = 50;
int const NOISE = -1;
int const TIME_MINUTES = 600;
double const SCAT_ANGLE = .18;

// MTTrackEpoch_852.csv MIN_PTS: 4 EPS: 2 SCAT_ANGLE: .15
// target.csv has wrong data
// fiveWashers.csv looks like 5 washers 610 minutes
// twoWashers.csv
// oneWasher.csv
// fiveWashers.csv high detail: MIN_PTS: 5 EPS: 1.5 TIME_MINUTES: 600 SCAT_ANGLE: .2
// MTTrackCombined.csv - box with objects TIME_MINUTES: 95
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif
