#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "greedy.h"

extern int size;

int isVisited(int *visited, int index){
    for(int i = 0; i < size; i++){
        if(*(visited + i) == index){
            return 1;
        }
    }
    return 0;
}

int findNearest(int currentCity, double *graph, int *visited) {
    double shortestPath = 999999999.0;
    int minIndex = 0;

    for (int i = 0; i < size; i++) {
        if (i != currentCity && isVisited(visited, i) == 0 && graph[i] < shortestPath) {
            minIndex = i;
            shortestPath = graph[i];
        }
    }

    return minIndex;
}

void greedy(double **graph, int startCity, char *city[100]) {
    int visited[size];
    memset(visited, -1, size * sizeof(int));

    double totalDistance = 0;
    int currentCity = startCity;
    visited[0] = startCity;

    for (int i = 1; i < size; i++) {
        visited[i] = findNearest(currentCity, graph[currentCity], visited);
        totalDistance += graph[currentCity][visited[i]];
        currentCity = visited[i];
    }
    totalDistance += graph[currentCity][startCity];

    printf("Best route found: ");
    for(int i = 0; i < size; i ++){
        printf("%s -> ", city[visited[i]]);
    }
    printf("%s", city[startCity]);
    printf("\nBest route distance: %lf km\n", totalDistance);
}
