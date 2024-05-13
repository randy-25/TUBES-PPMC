#ifndef GREEDY_H
#define GREEDY_H

extern int size;

int findNearest(int startCity, double **graph, int currentCity, int *visited, int numCity, int *totalDistance);
void greedy(double **graph, int startCity, char *city[100]);


#endif