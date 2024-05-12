#include <stdio.h>

#define SIZE 5

int findNearest(int startCity, int matrix[SIZE][SIZE], int currentCity, int *visited, int numCity, int *lengthPath){
    int shortestPath = 999999999;
    int nearestCity = -1;

    for(int i = 0; i < numCity; i++){
        if(i != startCity && visited[i] == 0 && matrix[currentCity][i] < shortestPath){
            nearestCity = i;
            shortestPath = matrix[currentCity][i];
        }
    }

    if (nearestCity != -1) {
        visited[nearestCity] = 1;
        *lengthPath += shortestPath;
    }

    return nearestCity;
}


void greedyTSP(int startCity, int matrix[SIZE][SIZE]){
    int visited[SIZE] = {0};
    char city[SIZE] = {'A', 'B', 'C', 'D', 'E'};
    int lengthPath = 0;
    
    int start = startCity - 1;

    printf("Jarak terpendek: %c -> ", city[start]);
    int currentCity = start;

    for(int i = 0; i < SIZE; i++){
        if(i == (SIZE - 1)){
            break;
        }

        int nearest = findNearest(start, matrix, currentCity, visited, SIZE, &lengthPath);
        currentCity = nearest;
        printf("%c -> ", city[nearest]);
    }
    lengthPath += matrix[currentCity][start];

    printf("%c \n", city[start]);
    printf("Dengan panjang: %d", lengthPath);
}

int main(){
    int startCity;
    printf("1 = A\n2 = B\n3 = C\n4 = D\n5 = E\n");
    printf("Masukkan kota awal: ");
    scanf("%d", &startCity);

    int matrix[SIZE][SIZE] = {
        {1, 2, 3, 4, 5},
        {2, 1, 6, 8, 3},
        {3, 6, 1, 5, 4},
        {4, 8, 5, 1, 2},
        {5, 4, 3, 2, 1}
    };

    greedyTSP(startCity, matrix);
}