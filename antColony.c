#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "antColony.h"

extern int size;

#define PI 3.141592653  // Pi constant
#define ALPHA 1         // faktor pentingnya jejak penanda
#define BETA 2          // faktor pentingnya informasi heuristik
#define RHO 0.5         // laju penguapan
#define Q 100           // Faktor deposit
#define MAX_ITER 1000   // Maximum number of iterations
#define NUM_ANTS 15  // Number of ants

void antCol(double **graph, int startingIndex, char *namaKota[100]){
    // memberi penanda pada setiap jalur yang ada
    double **Penanda = (double**)malloc(size * sizeof(double*));
    double **deltaPenanda = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        Penanda[i] = (double*)malloc(size * sizeof(double));
        deltaPenanda[i] = (double*)malloc(size * sizeof(double));
        for (int j = 0; j < size; j++) {
            Penanda[i][j] = 1.0; // Initial level penanda 
            deltaPenanda[i][j] = 0.0;
        }
    }

    int *bestTour = (int*)malloc(size * sizeof(int));
    double bestTourLength = -1;

    // Run Ant Colony Optimization
    srand(time(NULL)); // Initialize random seed
    antColonyOptimization(graph, Penanda, deltaPenanda, bestTour, &bestTourLength);

    // mencari urutan kota dengan rute terbaik
    int *bestTourIndices = (int*)malloc(size * sizeof(int));
    findBestTourIndices(bestTour, bestTourIndices, namaKota, size);

    // mencari indeks kota awal pada urutan kota dengan rute terbaik
    int startIndexInTour = -1;
    for (int i = 0; i < size; i++) {
        if (bestTour[i] == startingIndex) {
            startIndexInTour = i;
            break;
        }
    }

    // if (startIndexInTour == -1) {
    //     printf("Kota awal tidak ditemukan dalam tur terbaik.\n");
    //     return 0;
    // }

    printf("\nBest Tour starting from %s: ", namaKota[startingIndex]);
    for (int i = 0; i < size; i++) {
        printf("%s -> ", namaKota[bestTourIndices[(startIndexInTour + i) % size]]);
    }
    printf("%s\n", namaKota[bestTourIndices[startIndexInTour]]); // Print kota awal sebagai akhir rute

    printf("Best Tour Length: %lf km\n", bestTourLength);

    // Deallocate memory
    free(bestTourIndices);
    for (int i = 0; i < size; i++) {
        free(Penanda[i]);
        free(deltaPenanda[i]);
    }
    free(Penanda);
    free(deltaPenanda);
}

void updatePenanda(double **Penanda, double **deltaPenanda, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Penanda[i][j] = (1 - RHO) * Penanda[i][j] + deltaPenanda[i][j];
        }
    }
}

//jarak yang di tempuh setiap semut
void calculateDeltaPenanda(double **deltaPenanda, double *antDistances) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            deltaPenanda[i][j] = 0;
        }
    }
    for (int k = 0; k < NUM_ANTS; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                deltaPenanda[i][j] += Q / antDistances[k];
            }
        }
    }
}

void antColonyOptimization(double **graph, double **Penanda, double **deltaPenanda, int *bestTour, double *bestTourLength) {
    double *antDistances = (double*)malloc(NUM_ANTS * sizeof(double)); // untuk menyimpnan data panjang rute yang dilalui
    double *Preference = (double*)malloc(size * sizeof(double)); //preferensi jalur yang akan di ambil
    int *visited = (int*)malloc(size * sizeof(int)); // variabel untuk jalur yang sudah dikunjingi
    int *tour = (int*)malloc(size * sizeof(int)); //urutan kota yang dikunjungi

    for (int iteration = 0; iteration < MAX_ITER; iteration++) {
        for (int ant = 0; ant < NUM_ANTS; ant++) { //looping hingga semua untuk semua semut
            // Inisialisasi
            memset(visited, 0, size * sizeof(int));
            tour[0] = rand() % size; // mengisi array dengan rute random
            visited[tour[0]] = 1;

            // membangun rute dengan memilihi kota selanjutnya berdasarkan probabilitas
            for (int step = 1; step < size; step++) {
                // menghitung preferensi
                double total = 0.0;
                // dan juga menghitung pengaruh penanda di semua kota yang belum di kunjungi
                for (int i = 0; i < size; i++) {
                    if (!visited[i]) {
                        Preference[i] = pow(Penanda[tour[step - 1]][i], ALPHA) * pow(1.0 / graph[tour[step - 1]][i], BETA);
                        total += Preference[i];
                    } else {
                        Preference[i] = 0.0;
                    }
                }

                // Roulette wheel selection digunakan untuk acuan memilih kota berikutnya
                // yang mana  menjamin pemilihan probabilistik berdasarkan pengaruh penanda
                double rnd = (double)rand() / RAND_MAX * total;
                double cumulative = 0.0;
                int next = -1;
                for (int i = 0; i < size; i++) {
                    if (!visited[i]) {
                        cumulative += Preference[i];
                        if (cumulative >= rnd) {
                            next = i;
                            break;
                        }
                    }
                }

                if (next == -1) {
                    // If no city was selected, choose a random one
                    for (int i = 0; i < size; i++) {
                        if (!visited[i]) {
                            next = i;
                            break;
                        }
                    }
                }

                tour[step] = next;
                visited[next] = 1;
            }

            // menghitung total distance
            double distance = 0.0;
            for (int i = 0; i < size - 1; i++) {
                distance += graph[tour[i]][tour[i + 1]];
            }
            distance += graph[tour[size - 1]][tour[0]]; // Complete the tour

            antDistances[ant] = distance;

            // mengabdate tour terbaik
            if (distance < *bestTourLength || *bestTourLength == -1) {
                *bestTourLength = distance;
                memcpy(bestTour, tour, size * sizeof(int));
            }
        }

        // Update delta Penanda
        calculateDeltaPenanda(deltaPenanda, antDistances);

        // Update penanda
        updatePenanda(Penanda, deltaPenanda, size);
    }

    free(antDistances);
    free(Preference);
    free(visited);
    free(tour);
}

void findBestTourIndices(int *bestTour, int *bestTourIndices, char *namaKota[100], int size) {
    for (int i = 0; i < size; i++) {
        bestTourIndices[i] = bestTour[i];
    }
}