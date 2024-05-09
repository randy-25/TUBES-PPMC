#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printArr(char *namaKota[100], double *lintang, double *bujur, int size){
    for(int i = 0; i < size; i++){
        printf("%s , %lf, %lf\n", namaKota[i], lintang[i], bujur[i]);
    }
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
    int size;
    bacaFile(namaKota, &lintang, &bujur, file, &size);
    printArr(namaKota, lintang, bujur, size);
    return 0;
}