#ifndef GREEDY_H
#define GREEDY_H

extern int size;
int findVisited(int *visited, int index);
double minNeighbourIndex(double *graph, int currentIndex, int *visited);
void greedy(double **graph, int startVertex, char *namaKota[100]);

#endif