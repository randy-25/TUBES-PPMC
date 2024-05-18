#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "genetic.h"

#define POP_SIZE 100
#define GENERATIONS 500
#define MUTATION_RATE 0.1
#define TOURNAMENT_SIZE 5

extern int size;

double fitness(int *route, double **graph) {
    double totalDistance = 0;
    for (int i = 0; i < size - 1; i++) {
        totalDistance += graph[route[i]][route[i + 1]];
    }
    totalDistance += graph[route[size - 1]][route[0]];
    return totalDistance;
}

void generateRandomRoute(int *route) {
    for (int i = 0; i < size; i++) {
        route[i] = i;
    }
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = route[i];
        route[i] = route[j];
        route[j] = temp;
    }
}

void crossover(int *parent1, int *parent2, int *child) {
    int start = rand() % size;
    int end = rand() % size;
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    int visited[size];
    memset(visited, 0, sizeof(visited));
    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
        visited[parent1[i]] = 1;
    }
    int index = (end + 1) % size;
    for (int i = 0; i < size; i++) {
        int city = parent2[(end + 1 + i) % size];
        if (!visited[city]) {
            child[index] = city;
            index = (index + 1) % size;
        }
    }
}

void mutate(int *route) {
    if ((double)rand() / RAND_MAX < MUTATION_RATE) {
        int i = rand() % size;
        int j = rand() % size;
        int temp = route[i];
        route[i] = route[j];
        route[j] = temp;
    }
}

void tournamentSelection(int **population, double **graph, int *best) {
    int bestIndex = rand() % POP_SIZE;
    for (int i = 1; i < TOURNAMENT_SIZE; i++) {
        int index = rand() % POP_SIZE;
        if (fitness(population[index], graph) < fitness(population[bestIndex], graph)) {
            bestIndex = index;
        }
    }
    memcpy(best, population[bestIndex], size * sizeof(int));
}

void geneticAlgorithm(double **graph, int kotaAwal, char *namaKota[100]) {
    int **population = (int **)malloc(POP_SIZE * sizeof(int *));
    for (int i = 0; i < POP_SIZE; i++) {
        population[i] = (int *)malloc(size * sizeof(int));
        generateRandomRoute(population[i]);
    }
    
    int *newPopulation[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++) {
        newPopulation[i] = (int *)malloc(size * sizeof(int));
    }

    for (int generation = 0; generation < GENERATIONS; generation++) {
        for (int i = 0; i < POP_SIZE; i++) {
            int parent1[size], parent2[size];
            tournamentSelection(population, graph, parent1);
            tournamentSelection(population, graph, parent2);
            crossover(parent1, parent2, newPopulation[i]);
            mutate(newPopulation[i]);
        }

        for (int i = 0; i < POP_SIZE; i++) {
            memcpy(population[i], newPopulation[i], size * sizeof(int));
        }
    }

    int *bestRoute = population[0];
    double bestDistance = fitness(bestRoute, graph);
    for (int i = 1; i < POP_SIZE; i++) {
        double currentDistance = fitness(population[i], graph);
        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestRoute = population[i];
        }
    }

    int indexKotaAwal = 0;
    while (bestRoute[indexKotaAwal] != kotaAwal) {
        indexKotaAwal++;
    }
    int *finalRoute = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        finalRoute[i] = bestRoute[(indexKotaAwal + i) % size];
    }
    free(bestRoute);

    printf("Best route found: \n");
    for (int i = 0; i < size; i++) {
        printf("%s-> ", namaKota[finalRoute[i]]);
    }
    printf("%s", namaKota[kotaAwal]);
    printf("\nBest route distance: %lf\n", bestDistance);

    free(population);
    for (int i = 0; i < POP_SIZE; i++) {
        free(newPopulation[i]);
    }
    free(finalRoute);
}