#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "greedy.h"

extern int size;

int findNearest(int startCityCity, double **graph, int currentCity, int *visited, int numCity, double *totalDistance) {
    double shortestPath = 999999999.0;
    int nearestCity = -1;

    for (int i = 0; i < numCity; i++) {
        if (i != startCityCity && visited[i] == 0 && graph[currentCity][i] < shortestPath) {
            nearestCity = i;
            shortestPath = graph[currentCity][i];
        }
    }

    if (nearestCity != -1) {
        visited[nearestCity] = 1;
        *totalDistance += shortestPath;
    }

    return nearestCity;
}

void greedy(double **graph, int startCity, char *city[100]) {
    int visited[size];
    memset(visited, 0, sizeof(visited));
    double totalDistance = 0;

    printf("Best route found: \n%s -> ", city[startCity]);
    int currentCity = startCity;

    for (int i = 0; i < size - 1; i++) {
        int nearest = findNearest(startCity, graph, currentCity, visited, size, &totalDistance);
        if (nearest == -1) {
            printf("No path found from %s\n", city[currentCity]);
            return;
        }
        currentCity = nearest;
        printf("%s -> ", city[nearest]);
    }
    totalDistance += graph[currentCity][startCity];

    printf("%s \n", city[startCity]);
    printf("Best route distance: %lf km\n", totalDistance);
}
