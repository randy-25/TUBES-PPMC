#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "greedy.c"
#include "bruteforce.c"
#include "bfs.c"
#include "dfs.c"

#define PI 3.141592653  // Pi constant

int size;

double haversine(double lintang1, double bujur1, double lintang2, double bujur2);
void bacaFile(char *namaKota[100], double **lintang, double **bujur, FILE *file, int *size);
void printArr(char *namaKota[100], double *lintang, double *bujur);
void addGraph(double *lintang, double *bujur, double ***graph);
void printGraph(double **graph);
int findCityIndex(char *city, char *namaKota[100], int size);
int checkFileName(char *namaFile);


int main(){
    char namaFile[100];

    printf("Masukkan File: ");
    // scanf("%s", namaFile);
    fgets(namaFile, 100, stdin);
    namaFile[strcspn(namaFile, "\n")] = '\0';

    // cek format nama file
    int hasilCek = checkFileName(namaFile);
    if(hasilCek == 1){
        return 0;
    }

    char directory[100] = "./dataKota/";
    strcat(directory, namaFile);

    // Pembacaan File dan Validasi File
    FILE *file = fopen(directory, "r");
    // Cek apakah file ada
    if (file == NULL) {
        printf("File tidak ada!\n");
        return 0;
    }

    // cek apakah file kosong
    int lenFile;
    fseek(file, 0, SEEK_END);

    lenFile = ftell(file);
    if (lenFile == 0){
        printf("File kosong\n");
        return 0;
    }

    fseek(file, 0, SEEK_SET); // set kursor kembali ke awal file
    // Akhir pembacaan file dan validasi file

    // Pembacaan Map
    char *namaKota[100];
    double *lintang = NULL;
    double *bujur = NULL;
    
    bacaFile(namaKota, &lintang, &bujur, file, &size);
    printf("ISI FILE\n");
    printArr(namaKota, lintang, bujur);

    // alokasi memori graph
    double **graph = (double**)malloc(size*sizeof(double*));
    for(int i = 0; i < size; i++){
        graph[i] = (double*)malloc(size*sizeof(double));
    }
    addGraph(lintang, bujur, &graph);
    // printf("\nGraph\n");
    // printGraph(graph);

    char startingCity[100];
    printf("\nEnter starting point: ");
    fgets(startingCity, 100, stdin);
    startingCity[strcspn(startingCity, "\n")] = '\0';

    int startVertex = findCityIndex(startingCity, namaKota, size);
    if (startVertex == -1) {
        printf("Starting city not found!\n");
        return 0;
    }
    
    // Pemilihan Algoritma
    printf("Pilih Algoritma: \n");
    printf("1. Greedy\n");
    printf("2. Brute Force\n");
    printf("3. BFS\n");
    printf("4. DFS\n");
    printf("Pilihan: ");
    int pilihan;
    scanf("%d", &pilihan);
    if(pilihan < 1 || pilihan > 4){
        printf("Pilihan tidak valid\n");
        return 0;
    }

    // Algoritma dan Pengukuran Waktu
    clock_t start, end;
    switch (pilihan){
        case 1:
            start = clock();
            greedy(graph, startVertex, namaKota);
            end = clock();
            break;
        case 2:
            start = clock();
            bruteForce(graph, startVertex, namaKota);
            end = clock();
            break;
        case 3:
            start = clock();
            BFS(graph, namaKota, startVertex);
            end = clock();
            break;
        case 4:
            start = clock();
            DFS(graph, namaKota, startVertex);
            end = clock();
            break;
    }

    double timeElapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.10lf s\n", timeElapsed);
    
    getchar();
    char key;
    printf("Press any key to continue...");
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

int checkFileName(char *namaFile){
    // cek pertama ada format file atau tidak
    if (strchr(namaFile, '.') == NULL) {
        printf("Berikan format file!!!\n");
        return 1;
    }
    // cek jika ada spasi
    if(strchr(namaFile, ' ') != NULL){
        printf("Nama file tidak boleh mengandung spasi!!!\n");
        return 1;
    }
    char tokenStr[100];
    strcpy(tokenStr, namaFile);
    char *token;

    token = strtok(tokenStr, ".");
    token = strtok(NULL, ".");
    // cek format file
    if(strcmp(token, "csv") != 0){
        printf("Format file salah");
        return 1;
    }
}