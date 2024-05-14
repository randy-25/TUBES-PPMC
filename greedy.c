#include <stdlib.h>
#include <stdio.h>
#include "greedy.h"
#include <string.h>

extern int size;

// #define size 5

int findNearest(int startCity, double *graph, int currentCity, int *visited, int numCity, int *totalDistance){
    double shortestPath = 999999999.0;
    int nearestCity = -1;

    for(int i = 0; i < numCity; i++){
        if(i != startCity && visited[i] == 0 && graph[i] < shortestPath){
            nearestCity = i;
            shortestPath = graph[i];
        }
    }

    if (nearestCity != -1) {
        visited[nearestCity] = 1;
        *totalDistance += shortestPath;
    }

    return nearestCity;
}

void greedy(double **graph, int startCity, char *city[100]){
    int *visited = (int*)malloc(size*sizeof(int));
    memset(visited, 0, size * sizeof(int));
    int totalDistance = 0;
    
    int start = startCity - 1;

    printf("Jarak terpendek: %c -> ", city[start]);
    int currentCity = start;

    for(int i = 0; i < size; i++){
        if(i == (size - 1)){
            break;
        }

        int nearest = findNearest(start, graph[currentCity], currentCity, visited, size, &totalDistance);
        currentCity = nearest;
        printf("%s -> ", city[nearest]);
    }
    totalDistance += graph[currentCity][start];

    printf("%c \n", city[start]);
    printf("Dengan panjang: %d", totalDistance);

    free(visited);
}