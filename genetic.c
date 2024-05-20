// Nama: Muhammad Sheva Hermanto Putra
// NIM: 13222010
// Deskripsi Program: Algoritma Genetika

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "genetic.h"

#define POPULATION_SIZE 200
#define GENERATIONS 200
#define MUTATION_RATE 0.3
#define SELECTION_SIZE 30

extern int size;

double fitness(int *route, double **graph) {
    // Menghitung jarak sebuah rute
    double totalDistance = 0;
    for (int i = 0; i < size - 1; i++) {
        totalDistance += graph[route[i]][route[i + 1]];
    }
    totalDistance += graph[route[size - 1]][route[0]];
    return totalDistance;
}

void randomSolution(int *route) {
    for (int i = 0; i < size; i++) {
        route[i] = i;
    }
    // Solusi random dibuat dengan menukar urutan kota secara random
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = route[i];
        route[i] = route[j];
        route[j] = temp;
    }
}

void crossover(int *parent1, int *parent2, int *child) {
    // Ambil potongan rute urut acak dari parent1
    int start = rand() % size;
    int end = rand() % size;
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    // Buat array yang menandai suatu kota sudah ada dalam rute atau belum
    int visited[size];
    memset(visited, 0, sizeof(visited));
    // Memasukkan potongan parent1 ke individu baru
    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
        visited[parent1[i]] = 1;
    }
    // Masukkan kota dari parent2 untuk yang belum ada pada individu baru
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
    // Melakukan mutasi sesuai dengan mutation rate
    if ((rand() % 100) / 100 < MUTATION_RATE) {
        int i = rand() % size;
        int j = rand() % size;
        int temp = route[i];
        route[i] = route[j];
        route[j] = temp;
    }
}

void selection(int **population, double **graph, int *best) {
    // Mmeilih beberapa individu random yang kemudian akan dipilih individu dengan jarak terpendek
    int bestIndex = rand() % POPULATION_SIZE;
    for (int i = 1; i < SELECTION_SIZE; i++) {
        int index = rand() % POPULATION_SIZE;
        if (fitness(population[index], graph) < fitness(population[bestIndex], graph)) {
            bestIndex = index;
        }
    }
    memcpy(best, population[bestIndex], size * sizeof(int));
}

void geneticAlgorithm(double **graph, int kotaAwal, char *namaKota[100]) {
    // Inisiasi populasi awal berisi solusi acak
    int **population = (int **)malloc(POPULATION_SIZE * sizeof(int *));
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = (int *)malloc(size * sizeof(int));
        randomSolution(population[i]);
    }
    
    int *newPopulation[POPULATION_SIZE];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        newPopulation[i] = (int *)malloc(size * sizeof(int));
    }
    // Kembangkan populasi awal menjadi populasi baru sebanyak generasi yang diinginkan
    for (int generation = 0; generation < GENERATIONS; generation++) {
        for (int i = 0; i < POPULATION_SIZE; i++) {
            // Pengembangan dilakukan dengan cara seleksi, crossover, dan mutasi
            int parent1[size], parent2[size];
            selection(population, graph, parent1);
            selection(population, graph, parent2);
            crossover(parent1, parent2, newPopulation[i]);
            mutate(newPopulation[i]);
        }

        for (int i = 0; i < POPULATION_SIZE; i++) {
            memcpy(population[i], newPopulation[i], size * sizeof(int));
        }
    }
    // Diperoleh populasi terbaik kemudian cari solusi terbaik dalam populasi tersebut
    int *bestRoute = population[0];
    double bestDistance = fitness(bestRoute, graph);
    for (int i = 1; i < POPULATION_SIZE; i++) {
        double currentDistance = fitness(population[i], graph);
        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestRoute = population[i];
        }
    }
    // Susun solusi sesuai dengan kota awal yang diinput
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
    for (int i = 0; i < POPULATION_SIZE; i++) {
        free(newPopulation[i]);
    }
    free(finalRoute);
}
