#ifndef GENETIC_H_
#define GENETIC_H_
#include "genetic.c"
extern int size;

double fitness(int *route, double **graph);
void generateRandomRoute(int *route);
void crossover(int *parent1, int *parent2, int *child);
void mutate(int *route);
void tournamentSelection(int **population, double **graph, int *best);
void geneticAlgorithm(double **graph, int kotaAwal, char *namaKota[100]);


#endif // GENETIC_H_