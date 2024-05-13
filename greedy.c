#include <stdlib.h>
#include <stdio.h>
#include "greedy.h"

extern int size;

// fungsi untuk mencari apakah sebuah kota telah dikunjungi
int findVisited(int *visited, int index){
    for(int i = 0; i < size; i++){
        if (visited[i] == index){
            return 1;
        }
    }
    return 0;
}

//mencari index minimum dari tetangga yang belum dikunjungi
double minNeighbourIndex(double *graph, int currentIndex, int *visited){
    int minIndex = 0;
    double min = 999999.99;
    for(int i = 0; i < size; i++){
        if (graph[i] < min && findVisited(visited, i) == 0 && i != currentIndex){
            minIndex = i;
            min = graph[i];
        }
    }
    return minIndex;
}

//fungsi greedy utama
void greedy(double **graph, int startVertex, char *namaKota[100]){
    int *visited = (int*)malloc(size*sizeof(int));
    for(int i = 0; i < size; i++){
        visited[i] = -1;
    }
    double totalDistance = 0;
    visited[0] = startVertex;
    int currentVertex = startVertex;
    for(int i = 1; i < size; i++){
        visited[i] = minNeighbourIndex(graph
        [currentVertex], currentVertex, visited);
        totalDistance += graph[currentVertex][visited[i]];
        currentVertex = visited[i];
    }
    totalDistance += graph[currentVertex][startVertex];
    printf("Total Distance: %lf km\n", totalDistance);
    for(int i = 0; i < size; i++){
        printf("%s -> ", namaKota[visited[i]]);
    }
    printf("%s\n", namaKota[startVertex]);
}