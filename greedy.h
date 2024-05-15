#ifndef GREEDY_H_
#define GREEDY_H_

extern int size;

int findNearest(int startCity, double **graph, int currentCity, int *visited, int numCity, double *totalDistance);
void greedy(double **graph, int startCity, char *city[100]);


#endif