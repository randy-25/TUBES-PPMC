#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "greedy.h"

#define PI 3.141592653  // Pi constant

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
  double distance = 2*R*asin(sqrt((sin(dLat / 2) * sin(dLat / 2) + cos(lintang1) * cos(lintang2) * sin(dLon / 2) * sin(dLon / 2))));

  return distance; // in km
}


void bacaFile(char *namaKota[100], double **lintang, double **bujur, FILE *file, int *size) {
    char *stringLine;
    while(fgets(stringLine, 300, file) != NULL){
        namaKota[*size] = (char*)malloc(sizeof(char));
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
                (*graph)[i][j] = 0.0;
            }else if (j < i){ // ketika sudah dihitung jarak antar 2 kota
                (*graph)[i][j] = (*graph)[j][i];
            }else{ // menghitung jarak 2 kota
                (*graph)[i][j] = haversine(lintang[i], bujur[i], lintang[j], bujur[j]);
            }            
        }
    }
}


int findCityIndex(char *cityName, char *namaKota[100], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(cityName, namaKota[i]) == 0) {
            return i;
        }
    }
    return -1; // Jika nama kota tidak ditemukan
}


int main(){
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
    // char directory[100] = "./dataKota/";
    // strcat(directory, namaFile);

    FILE *file = fopen(namaFile, "r");

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

    char startingCity[100];
    printf("\nEnter starting point: ");
    scanf("%s", startingCity);

    int startVertex = findCityIndex(startingCity, namaKota, size);
    if (startVertex == -1) {
        printf("Starting city not found!\n");
        return 0;
    }

    printf("size: %d\n", size);
    // bfsTree* root = NULL;

    clock_t start = clock();

    
    // Greedy
    greedy(graph, startVertex, namaKota);

    // Brute Force
    // bruteForce(graph, startVertex, namaKota);

    //BFS
    // BFS(graph, namaKota, startVertex);
    
    //DFS
    // DFS(graph, namaKota, startVertex);
    clock_t end = clock();

    double timeElapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.10lf s\n", timeElapsed);
    

    char key;
    printf("Press any key to continue...");
    scanf("%c", &key);
    scanf("%c", &key);
        
    //deallocate
    free(lintang);
    free(bujur);
    for(int i = 0; i < size; i++){
        free(graph[i]);
    }
    free(graph);
    fclose(file);
    
    return 0;
}