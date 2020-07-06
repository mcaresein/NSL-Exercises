#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include <vector>

int NUMBER_OF_CITIES;
int NUMBER_OF_GENERATIONS;
int NUMBER_OF_PATHS;
int MOVES_PER_TEMPERATURE;
double T_MIN;
double T_MAX;
double T_STEP ;
bool VERBOSE;

std::vector< std::vector<double> > x_of_cities, y_of_cities;
std::vector<double> probabilities;
double beta, temperature;

int accepted, attempted;
int iprint=5;

#endif
