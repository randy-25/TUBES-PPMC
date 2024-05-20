/*
Nama : Randy Revaldo Pratama
NIM  : 13222012
Header File dari bruteforce.c untuk penghubung ke main program
*/
#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

extern int size;

void swap(int *x, int *y);
void heapPermutation(int *a, int n, int sizePerm, double **graph, int *currentPath, double *currentDistance, int startVertex);
void bruteForce(double **graph, int startVertex, char *namaKota[100]);

#endif