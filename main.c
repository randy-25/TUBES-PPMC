#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "branchnbound.h"

#define PI 3.141592653  // Pi constant
#define N 100

int size = 0;

// Function to calculate distance between two cities using Haversine formula
double haversine(double lintang1, double bujur1, double lintang2, double bujur2) {
    double R = 6371; // Earth's Radius in KM
    
    // Convert latitudes and longitudes to radians
    lintang1 = lintang1 * PI / 180;
    bujur1 = bujur1 * PI / 180;
    lintang2 = lintang2 * PI / 180;
    bujur2 = bujur2 * PI / 180;

    // Calculate the difference in latitude and longitude
    double dLat = lintang2 - lintang1;
    double dLon = bujur2 - bujur1;

    // Haversine formula
    double distance = 2 * R * asin(sqrt((sin(dLat / 2) * sin(dLat / 2) + cos(lintang1) * cos(lintang2) * sin(dLon / 2) * sin(dLon / 2))));

    return distance; // in km
}

// Function to read data from file
void bacaFile(char *namaKota[N], double **lintang, double **bujur, FILE *file) {
    char stringLine[300];
    while (fgets(stringLine, 300, file) != NULL) {
        namaKota[size] = (char*)malloc(strlen(stringLine) * sizeof(char));
        if ((*lintang) == NULL && (*bujur) == NULL) {
            (*lintang) = (double*)malloc(sizeof(double));
            (*bujur) = (double*)malloc(sizeof(double));
        } else {
            (*lintang) = (double*)realloc((*lintang), (size + 1) * sizeof(double));
            (*bujur) = (double*)realloc((*bujur), (size + 1) * sizeof(double));
        }
        // Parse the string
        char *temp;
        temp = strtok(stringLine, ",");
        strcpy(namaKota[size], temp);
        temp = strtok(NULL, ",");
        (*lintang)[size] = atof(temp); // Convert to double
        temp = strtok(NULL, ",");
        (*bujur)[size] = atof(temp); // Convert to double
        size++;
    }
}

// Function to print CSV content
void printData(char *namaKota[N], double *lintang, double *bujur) {
    for (int i = 0; i < size; i++) {
        printf("%s, %lf, %lf\n", namaKota[i], lintang[i], bujur[i]);
    }
}

// Function to add data to graph
void addGraph(double *lintang, double *bujur, double ***graph){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i == j){ // kalau sama
                (*graph)[i][j] = 0.0;
            }else if (j < i){ // ketika sudah dihitung jarak antar 2 kota
                (*graph)[i][j] = (*graph)[j][i];
            }else{ // menghitung jarak 2 kota
                (*graph)[i][j] = haversine(lintang[i], bujur[i], lintang[j], bujur[j]);
            }            
        }
    }
}

// Function to print graph (debugging)
void printGraph(double **graph) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%lf ", graph[i][j]);
        }
        printf("\n");
    }
}

// Function to find city index
int findCityIndex(char *cityName, char *namaKota[100], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(cityName, namaKota[i]) == 0) {
            return i;
        }
    }
    return -1; // Jika nama kota tidak ditemukan
}


int main() {
    char namaFile[100];

    printf("Enter File Name: ");
    scanf("%s", namaFile);
    char *token;
    char tokenStr[100];
    strcpy(tokenStr, namaFile);

    token = strtok(tokenStr, ".");
    token = strtok(NULL, ".");
    // Check file format
    if (strcmp(token, "csv") != 0) {
        printf("Invalid file format!\n");
        return 0;
    }

    FILE *file = fopen(namaFile, "r");
    // Check if file exists
    if (file == NULL) {
        printf("File tidak ada!\n");
        return 0;
    }

    int lenFile;
    fseek(file, 0, SEEK_END);
    // Check if file is empty
    lenFile = ftell(file);
    if (lenFile == 0) {
        printf("File kosong\n");
        return 0;
    }

    fseek(file, 0, SEEK_SET);
    char *namaKota[N];
    double *lintang = NULL;
    double *bujur = NULL;

    bacaFile(namaKota, &lintang, &bujur, file);
    printf("\nFile Content\n");
    printData(namaKota, lintang, bujur);

    // Create and print graph
    double **graph = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        graph[i] = (double*)malloc(size * sizeof(double));
    }
    addGraph(lintang, bujur, &graph);
    printf("\nGraph\n");
    printGraph(graph);

    // Prompt user to enter starting city
    char startCity[100];
    printf("\nEnter starting point: ");
    scanf("%s", startCity);

    int startVertex = findCityIndex(startCity, namaKota, size);
    if (startVertex == -1) {
        printf("Starting city not found!\n");
        return 0;
    }

    // BranchnBound
    clock_t start = clock();
    BranchnBound(graph, startVertex, namaKota);
    clock_t end = clock();

    // Print the time elapsed
    double timeElapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.10lf s\n", timeElapsed);

    // Free memory and close file
    free(lintang);
    free(bujur);
    for (int i = 0; i < size; i++) {
        free(graph[i]);
    }
    free(graph);
    fclose(file);

    return 0;
}
