#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.141592653  // Pi constant
#define ALPHA 1         // faktor pentingnya jejak penanda
#define BETA 2          // faktor pentingnya informasi heuristik
#define RHO 0.5         // laju penguapan
#define Q 100           // Faktor deposit
#define MAX_ITER 1000   // Maximum number of iterations
#define NUM_ANTS 15  // Number of ants

int size;

double haversine(double lintang1, double bujur1, double lintang2, double bujur2) {
    double R = 6371; // Earth's Radius in KM
  
    // Convert latitudes and longitudes to radian
    lintang1 = lintang1 * PI / 180;
    bujur1 = bujur1 * PI / 180;
    lintang2 = lintang2 * PI / 180;
    bujur2 = bujur2 * PI / 180;

    // Calculate the difference in latitude and longitude
    double dLat = lintang2 - lintang1;
    double dLon = bujur2 - bujur1;

    // Haversine formula steps
    double distance = 2 * R * asin(sqrt((sin(dLat / 2) * sin(dLat / 2) + cos(lintang1) * cos(lintang2) * sin(dLon / 2) * sin(dLon / 2))));

    return distance; // in km
}

void bacaFile(char *namaKota[100], double **lintang, double **bujur, FILE *file, int *size) {
    char stringLine[300]; // Allocate space for stringLine
    while(fgets(stringLine, 300, file) != NULL){
        namaKota[*size] = (char*)malloc(strlen(stringLine) + 1); // Allocate space according to string length
        if ((*lintang) == NULL && (*bujur) == NULL){
            (*lintang) = (double*)malloc(sizeof(double));
            (*bujur) = (double*)malloc(sizeof(double));
        }else{
            (*lintang) = (double*)realloc((*lintang), ((*size)+1)*sizeof(double));
            (*bujur) = (double*)realloc((*bujur), ((*size)+1)*sizeof(double));
        }
        
        // parse the string
        char *temp;
        temp = strtok(stringLine, ",");
        strcpy(namaKota[*size], temp);
        temp = strtok(NULL, ",");
        (*lintang)[*size] = atof(temp); //convert to double
        temp = strtok(NULL, ",");
        (*bujur)[*size] = atof(temp); //convert to double
        (*size)++;
    }
}

void printArr(char *namaKota[100], double *lintang, double *bujur){
    for(int i = 0; i < size; i++){
        printf("%s , %lf, %lf\n", namaKota[i], lintang[i], bujur[i]);
    }
}

void printGraph(double **graph){
    for(int i = 0; i < size; i++){
        for(int j = 0; j< size; j++){
            printf("%lf ", graph[i][j]);
        }
        printf("\n");
    }
}

void addGraph(double *lintang, double *bujur, double ***graph){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i == j){ // kalau sama
                (*graph)[i][j] = 0.000000;
            }else if (j < i){ // ketika sudah dihitung jarak antar 2 kota
                (*graph)[i][j] = (*graph)[j][i];
            }else{ // menghitung jarak 2 kota
                (*graph)[i][j] = haversine(lintang[i], bujur[i], lintang[j], bujur[j]);
            }            
        }
    }
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

int findCityIndex(char *cityName, char *namaKota[100], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(cityName, namaKota[i]) == 0) {
            return i;
        }
    }
    return -1; // City not found
}

void findBestTourIndices(int *bestTour, int *bestTourIndices, char *namaKota[100], int size) {
    for (int i = 0; i < size; i++) {
        bestTourIndices[i] = bestTour[i];
    }
}

int main() {
    char namaFile[100];

    printf("Masukkan File: ");
    scanf("%s", namaFile);
    char tokenStr[100];
    strcpy(tokenStr, namaFile);
    char *token;

    token = strtok(tokenStr, ".");
    token = strtok(NULL, ".");
    // cek format file
    if(strcmp(token, "csv") != 0){
        printf("Format file salah");
        return 0;
    }
    char directory[100] = "./dataKota/";
    strcat(directory, namaFile);

    FILE *file = fopen(directory, "r");

    // Cek apakah file ada
    if (file == NULL) {
        printf("File tidak ada!\n");
        return 0;
    }

    int lenFile;
    fseek(file, 0, SEEK_END);

    lenFile = ftell(file);
    if (lenFile == 0){
        printf("File kosong\n");
        return 0;
    }

    fseek(file, 0, SEEK_SET);
    char *namaKota[100];
    double *lintang = NULL;
    double *bujur = NULL;
    
    bacaFile(namaKota, &lintang, &bujur, file, &size);
    printf("ISI FILE\n");
    printArr(namaKota, lintang, bujur);

    double **graph = (double**)malloc(size*sizeof(double*));
    for(int i = 0; i < size; i++){
        graph[i] = (double*)malloc(size*sizeof(double));
    }
    addGraph(lintang, bujur, &graph);
    printf("\nGraph\n");
    printGraph(graph);

    // memberi penanda pada setiap jalur yang ada
    // double **Penanda = (double**)malloc(size * sizeof(double*));
    // double **deltaPenanda = (double**)malloc(size * sizeof(double*));
    // for (int i = 0; i < size; i++) {
    //     Penanda[i] = (double*)malloc(size * sizeof(double));
    //     deltaPenanda[i] = (double*)malloc(size * sizeof(double));
    //     for (int j = 0; j < size; j++) {
    //         Penanda[i][j] = 1.0; // Initial level penanda 
    //         deltaPenanda[i][j] = 0.0;
    //     }
    // }

    // int *bestTour = (int*)malloc(size * sizeof(int));
    // double bestTourLength = -1;

    char startingCity[100];
    printf("Masukkan Kota Awal: ");
    scanf("%s", startingCity);

    // mencari indeks kota awal
    int startingIndex = findCityIndex(startingCity, namaKota, size);
    if (startingIndex == -1) {
        printf("Kota awal tidak ditemukan.\n");
        return 0;
    }

    // // Run Ant Colony Optimization
    // srand(time(NULL)); // Initialize random seed
    // antColonyOptimization(graph, Penanda, deltaPenanda, bestTour, &bestTourLength);

    // // mencari urutan kota dengan rute terbaik
    // int *bestTourIndices = (int*)malloc(size * sizeof(int));
    // findBestTourIndices(bestTour, bestTourIndices, namaKota, size);

    // mencari indeks kota awal pada urutan kota dengan rute terbaik
    // int startIndexInTour = -1;
    // for (int i = 0; i < size; i++) {
    //     if (bestTour[i] == startingIndex) {
    //         startIndexInTour = i;
    //         break;
    //     }
    // }
    // if (startIndexInTour == -1) {
    //     printf("Kota awal tidak ditemukan dalam tur terbaik.\n");
    //     return 0;
    // }
    
    clock_t start = clock();
    // Print the best tour dimulai dari kota awal
    // printf("\nBest Tour starting from %s: ", startingCity);
    // for (int i = 0; i < size; i++) {
    //     printf("%s -> ", namaKota[bestTourIndices[(startIndexInTour + i) % size]]);
    // }
    // printf("%s\n", namaKota[bestTourIndices[startIndexInTour]]); // Print kota awal sebagai akhir rute

    // printf("Best Tour Length: %.2lf km\n", bestTourLength);
    clock_t end = clock();
    // Deallocate memory
    free(lintang);
    free(bujur);
    // free(bestTourIndices);
    // for (int i = 0; i < size; i++) {
    //     free(graph[i]);
    //     free(Penanda[i]);
    //     free(deltaPenanda[i]);
    // }
    
    double timeElapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.10lf s\n", timeElapsed);
    free(graph);
    // free(Penanda);
    // free(deltaPenanda);
    fclose(file);

    return 0;
}
