/*
Nama : Randy Revaldo Pratama
NIM  : 13222012
Program untuk menyelesaikan TSP menggunakan algoritma Brute Force yaitu dengan mencoba semua kemungkinan path yang ada
*/

#include <stdio.h>
#include <stdlib.h>
#include "bruteforce.h"

extern int size;

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Generating permutation using Heap Algorithm
void heapPermutation(int *a, int n, int sizePerm, double **graph, int *currentPath, double *currentDistance, int startVertex)
{
    // if size becomes 1 then prints the obtained
    // permutation
    if (n == 1) {
        double tempDistance = 0;
        for(int i = 0; i < sizePerm-1; i++){
            tempDistance += graph[a[i]][a[i+1]];
        }
        tempDistance += graph[startVertex][a[0]];
        tempDistance += graph[a[sizePerm-1]][startVertex];
        if (tempDistance < *currentDistance){
            *currentDistance = tempDistance;
            for(int i = 1; i < sizePerm+1; i++){
                currentPath[i] = a[i-1];
            }
        }
    }
    for (int i = 0; i < n; i++) {
        heapPermutation(a, n - 1, sizePerm, graph, currentPath, currentDistance, startVertex);
        // if size is odd, swap 0th i.e (first) and 
        // (size-1)th i.e (last) element
        if (n % 2 == 1){
            swap(&a[0], &a[n- 1]);
        }
        // If size is even, swap ith and 
        // (size-1)th i.e (last) element
        else{
            swap(&a[i], &a[n- 1]);
        }
    }
}

void bruteForce(double **graph, int startVertex, char *namaKota[100]){
    int *currentPath = (int*)malloc(size*sizeof(int));
    for(int i = 0; i < size; i++){
        currentPath[i] = i;
    }
    swap(&currentPath[0], &currentPath[startVertex]);
    int *a = (int*)malloc((size-1)*sizeof(int));
    for(int i = 1; i < size; i++){
        a[i-1] = currentPath[i];
    }
    double currentDistance = 999999.99;
    heapPermutation(a, size-1, size-1, graph, currentPath, &currentDistance, startVertex);
    printf("Total Distance: %lf km\n", currentDistance);
    for(int i = 0; i < size; i++){
        printf("%s -> ", namaKota[currentPath[i]]);
    }
    printf("%s\n", namaKota[startVertex]);
}