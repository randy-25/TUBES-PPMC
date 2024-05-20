// Nama: Muhammad Sheva Hermanto Putra
// NIM: 13222010
// Deskripsi Program: Algoritma Genetika

#ifndef GENETIC_H_
#define GENETIC_H_

extern int size;

double fitness(int *route, double **graph);
void randomSolution(int *route);
void crossover(int *parent1, int *parent2, int *child);
void mutate(int *route);
void selection(int **population, double **graph, int *best);
void geneticAlgorithm(double **graph, int kotaAwal, char *namaKota[100]);


#endif // GENETIC_H_
