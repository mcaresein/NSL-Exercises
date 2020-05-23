#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include <vector>

int NUMBER_OF_CITIES;
int NUMBER_OF_GENERATIONS=100;
int NUMBER_OF_PATHS=10;
int MOVES_PER_TEMPERATURE=100;
double T_MIN=0.01;
double T_MAX=1.5;
double T_STEP=10 ;

std::vector< std::vector<double> > x_of_cities, y_of_cities;
std::vector<double> probabilities;
double beta, temperature;

int accepted, attempted;
int iprint=5;

#endif
