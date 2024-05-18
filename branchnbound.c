#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int size;

// Fungsi untuk mencari nilai minimum (distance) pertama
double firstMin(double **graph, int vertex) {
    double min = 999999.99;
    for (int j = 0; j < size; j++) {
        if (graph[vertex][j] <= min && vertex != j) {
            min = graph[vertex][j];
        }
    }
    return min;
}

// Fungsi untuk mencari nilai minimum (distance) kedua
double secondMin(double **graph, int vertex) {
    double first = 999999.99, second = 999999.99;
    for (int j = 0; j < size; j++) {
        if (vertex == j)
            continue;
        if (graph[vertex][j] <= first) {
            second = first;
            first = graph[vertex][j];
        }
        else if (graph[vertex][j] <= second && graph[vertex][j] != first) {
            second = graph[vertex][j];
        }
    }
    return second;
}

// Fungsi untuk menyalin array curr_path ke final_path
void copytoFinal(int curr_path[], int final_path[]) {
    for (int i = 0; i < size; i++)
        final_path[i] = curr_path[i];
    final_path[size] = curr_path[0];
}

// Algoritma utama Branch and Bound menggunakan rekursif
void BranchnBoundRec(double **graph, double *final_res, double curr_bound, double curr_weight, int level, int curr_path[], int final_path[], int visited[], int startvertex) {
    // Kasus basis ketika sudah mencapai level = size atau semua node sudah tercapai
    if (level == size) {
        // Mengecek apakah ada edge dari vertex terakhir ke startVertex
        if (graph[curr_path[level-1]][startvertex] != 0) {
            double curr_res = curr_weight + graph[curr_path[level-1]][startvertex];
            if (curr_res < *final_res) {
                copytoFinal(curr_path, final_path);
                *final_res = curr_res;
            }
        }
        return;
    }

    // Untuk level selain size, dilakukan iterasi untuk membuat search space tree
    for (int i = 0; i < size; i++) {
        if (graph[curr_path[level-1]][i] != 0 && visited[i] == 0) {
            double temp = curr_bound;
            curr_weight += graph[curr_path[level-1]][i];

            if (level == 1) {
                curr_bound -= ((firstMin(graph, curr_path[level-1]) + firstMin(graph, i)) /2);
            }
            else {
                curr_bound -= ((secondMin(graph, curr_path[level-1]) + firstMin(graph, i)) /2);
            }
            
            if (curr_bound + curr_weight < *final_res) {
                curr_path[level] = i;
                visited[i] = 1;

                BranchnBoundRec(graph, final_res, curr_bound, curr_weight, level+1, curr_path, final_path, visited, startvertex);
            }

            curr_weight -= graph[curr_path[level-1]][i];
            curr_bound = temp;

            for (int j = 0; j <= level; j++)
                visited[curr_path[j]] = 0;

            for (int j = 0; j < level; j++)
                visited[curr_path[j]] = 1;
        }
    }
}

void BranchnBound(double **graph, int startvertex, char *cityName[100]) {
    int visited[size];
    memset(visited, 0, sizeof(visited));
    int path[size+1];
    memset(path, -1, sizeof(path));
    int final_path[size+1];
    memset(final_path, 0, sizeof(final_path));
    double curr_bound = 0;
    double final_res = 99999999.99;

    for (int i = 0; i < size; i++) {
        curr_bound += (firstMin(graph, i) + secondMin(graph, i));
    }

    curr_bound = curr_bound/2;
    visited[startvertex] = 1;
    path[0] = startvertex;

    BranchnBoundRec(graph, &final_res, curr_bound, 0, 1, path, final_path, visited, startvertex);
    
    printf("Best route found:\n");
    printf("Path Taken: ");
    for (int i = 0; i <= size; i++) {
        printf("%s", cityName[final_path[i]]);
        if (i == size) {
            printf("\n");
        }
        else printf(" -> ");
    }
    printf("Best route distance: %lf km\n", final_res);
}