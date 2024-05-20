/*
Nama: Riswandha Mashuri
NIM: 13222002
Deskripsi Program: File header yang digunakan untuk menghubungkan main.c dan greedy.c
*/
#ifndef GREEDY_H_
#define GREEDY_H_

extern int size;

int isVisited(int *visited, int index);
int findNearest(int currentCity, double *graph, int *visited);
void greedy(double **graph, int startCity, char *city[100]);


#endif