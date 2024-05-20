/*
Nama : Randy Revaldo Pratama
NIM  : 13222012
Program ini adalah program sampingan sebagai antisipasi untuk BFS yang tidak dapat digunakan secara langsung jika tidak dijalankan dengan tombol run di vscode
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include "bfs.h"

typedef struct bfsTree
{
    int node;
    int *visited;
    int *queue;
    double totalDistance;
    int level;
    int completeChild;
    struct bfsTree **next; // Array of pointers to child nodes
} bfsTree;

bfsTree *createNode(int data);
void BFS(bfsTree **tree, double **adjMatrix, int startVertex);
bfsTree *createTree(bfsTree *root, double **adjMatrix, int startVertex);
void findMinDistance(bfsTree *tree, int startVertex, double **adjMatrix, char *namaKota[100]);
void printTree(bfsTree *tree);
void freeTree(bfsTree *tree);

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

    char directory[100] = "./dataKota/";
    strcat(directory, namaFile);

    FILE *file = fopen(directory, "r");

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
    
    clock_t start = clock();
    bfsTree* root = NULL;
    root = createTree(root, graph, startVertex);
    printf("root: %d\n", root->node);
    printTree(root);
    findMinDistance(root, startVertex, graph, namaKota);
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
bfsTree *createNode(int data)
{
    bfsTree *newNode = (bfsTree *)malloc(sizeof(bfsTree));
    newNode->node = data;
    newNode->visited = NULL;
    newNode->queue = NULL;
    newNode->totalDistance = 0.0;
    newNode->level = 0;
    newNode->completeChild = 0;
    newNode->next = NULL;
    return newNode;
}

int countDepth(int maxSize)
{
    maxSize--;
    int tempSize = maxSize;
    int count = 0;
    while (tempSize > 0)
    {
        int pengali = 1;
        for (int i = maxSize - tempSize; i > 0; i--)
        {
            pengali *= maxSize - i;
        }
        count += pengali * maxSize;
        tempSize--;
    }
    return count + 1;
}

void *findParent(bfsTree **root, int level, int visited)
{
    // Use a queue for BFS traversal
    int depth = countDepth(size);
    bfsTree **queue = (bfsTree **)malloc(sizeof(bfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = (*root);
    int check = 0;
    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node

        while (front != rear && queue[front]->level == currentLevel)
        {
            bfsTree *temp = queue[front++];
            if (temp->level + 1 == level && temp->node == visited && temp->completeChild == 0)
            {
                (*root) = temp;
                check = 1;
                break;
            }
            if (check == 0)
            {
                // Enqueue child nodes for the next level
                for (int i = 0; i < size - (temp->level + 1); i++)
                {
                    if (temp->next[i] != NULL)
                    {
                        queue[rear++] = temp->next[i];
                    }
                }
            }
        }
        if (check == 1)
        {
            break;
        }
    }
    free(queue);
}

bfsTree *createTree(bfsTree *root, double **adjMatrix, int startVertex)
{
    if (root == NULL)
    { // Create the root node
        root = createNode(startVertex);
    }
    BFS(&root, adjMatrix, startVertex); // for the first level
    // Use a queue to store nodes during level order traversal
    int depth = countDepth(size);
    bfsTree **queue = (bfsTree **)malloc(sizeof(bfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = root;
    bfsTree **track = (bfsTree **)malloc(sizeof(bfsTree *) * depth);
    int count = 0;

    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node
        while (front != rear && queue[front]->level == currentLevel)
        {
            bfsTree *temp = queue[front++];
            BFS(&temp, adjMatrix, temp->node); // Recursively traverse child nodes
            track[count++] = temp;
            track[count - 1]->completeChild = 0;
            // Enqueue child nodes for the next level
            for (int i = 0; i < size - (temp->level + 1); i++)
            {
                if (temp->next[i] != NULL)
                {
                    queue[rear++] = temp->next[i];
                }
            }
        }
    }
    int j = 0;
    root->next = NULL;
    root->completeChild = 0;
    for (int i = 0; i < count; i++)
    {
        bfsTree *tempRoot = root;
        int level = track[i]->level;
        if (level != 0)
        {
            int visited = track[i]->visited[track[i]->level - 1];
            findParent(&tempRoot, level, visited);
            if (tempRoot->next == NULL)
            {
                tempRoot = (bfsTree *)realloc(tempRoot, sizeof(bfsTree) + (size - (level)) * sizeof(bfsTree *));
                tempRoot->next = (bfsTree **)malloc(sizeof(bfsTree *) * (size - (level)));
            }
            tempRoot->next[j] = track[i];
            tempRoot->next[j]->next = NULL;
            if (j == size - (level + 1))
            {
                tempRoot->completeChild = 1;
                j = 0;
            }
            else
            {
                j++;
            }
        }
    }
    free(queue);
    free(track);
    return root;
}

void BFS(bfsTree **tree, double **adjMatrix, int startVertex)
{
    int visitedSize = (*tree)->level + 1;
    int queueSize = size - (*tree)->level - 1;
    (*tree) = realloc((*tree), sizeof(bfsTree) + (visitedSize + queueSize) * sizeof(int));
    if ((*tree)->visited == NULL)
    {
        (*tree)->visited = (int *)malloc(sizeof(int) * visitedSize);
    }
    (*tree)->visited[(*tree)->level] = startVertex; // Mark starting vertex as visited
    (*tree)->queue = (int *)malloc(sizeof(int) * queueSize);
    for (int i = 0; i < visitedSize - 1; i++)
    {
        (*tree)->totalDistance += adjMatrix[(*tree)->visited[i]][(*tree)->visited[i + 1]];
    }

    int k = 0;
    for (int i = 0; i < size; i++)
    { // Enqueue all unvisited neighbors of startVertex
        int check = 0;
        for (int j = 0; j < visitedSize; j++)
        {
            if (i == (*tree)->visited[j])
            {
                check++;
            }
        }
        if (check == 0)
        {
            (*tree)->queue[k] = i;
            k++;
        }
    }
    (*tree) = realloc((*tree), sizeof(bfsTree) + (queueSize) * sizeof(bfsTree *));
    (*tree)->next = (bfsTree **)malloc(sizeof(bfsTree *) * queueSize); // Allocate space for child pointers
    for (int i = 0; i < queueSize; i++)
    { // Process all nodes at the current level
        int currentNode = (*tree)->queue[i];
        (*tree)->next[i] = createNode(currentNode);   // Create child node
        (*tree)->next[i]->level = (*tree)->level + 1; // Set child level
        (*tree)->next[i]->visited = (int *)malloc(sizeof(int) * (visitedSize + 1));
        for (int j = 0; j < visitedSize; j++)
        {
            (*tree)->next[i]->visited[j] = (*tree)->visited[j]; // Copy visited nodes to child
        }
        (*tree)->next[i]->visited[visitedSize] = currentNode; // Mark current node as visited
        (*tree)->next[i]->queue = NULL;
        (*tree)->next[i]->next = NULL;
    }
}

double min(double a, double b)
{
    return (a < b) ? a : b;
}

void findMinDistance(struct bfsTree *tree, int startVertex, double **adjMatrix, char *namaKota[100])
{
    double minDistance = 9999999.99;
    int *shortestPath = (int *)malloc(sizeof(int) * (size + 1));
    if (tree == NULL)
    {
        return; // Base case: Empty tree
    }
    int depth = countDepth(size);
    // Use a queue to store nodes during level order traversal
    struct bfsTree **queue = (struct bfsTree **)malloc(sizeof(struct bfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = tree;

    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node

        // Print all nodes at the current level
        while (front != rear && queue[front]->level == currentLevel)
        {
            struct bfsTree *temp = queue[front++];
            if (temp->level == size - 1)
            {
                if (temp->totalDistance + adjMatrix[temp->visited[size - 1]][startVertex] < minDistance)
                {
                    minDistance = temp->totalDistance + adjMatrix[temp->visited[size - 1]][startVertex];
                    for (int i = 0; i < size; i++)
                    {
                        shortestPath[i] = temp->visited[i];
                    }
                    shortestPath[size] = startVertex;
                }
            }

            // Enqueue child nodes for the next level
            for (int i = 0; i < size - (temp->level + 1); i++)
            {
                // printf("i %d tempNext %d\n",i, temp->next[i]->node);
                if (temp->next[i] != NULL)
                {
                    queue[rear++] = temp->next[i];
                }
            }
        }
    }
    printf("Min Distance: %lf\n", minDistance);
    printf("The shortest path is: ");
    for (int i = 0; i < size; i++)
    {
        printf("%s -> ", namaKota[shortestPath[i]]);
    }
    printf("%s\n", namaKota[shortestPath[size]]);
    printf("\n");
    free(queue);
}

void printTree(bfsTree *tree)
{
    if (tree == NULL)
    {
        return; // Base case: Empty tree
    }
    int depth = countDepth(size);
    // Use a queue to store nodes during level order traversal
    bfsTree **queue = (bfsTree **)malloc(sizeof(bfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = tree;

    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node

        // Print all nodes at the current level
        while (front != rear && queue[front]->level == currentLevel)
        {
            bfsTree *temp = queue[front++];
            printf("Node: %d, Level: %d\n", temp->node, temp->level);
            printf("Visited: ");
            for (int i = 0; i <= temp->level; i++)
            {
                printf("%d ", temp->visited[i]);
            }
            printf("\n");
            printf("Total Distance: %.2f\n", temp->totalDistance);
            printf("Child: ");
            for (int i = 0; i < size - (temp->level + 1); i++)
            {
                if (temp->next[i] != NULL)
                {
                    printf("%d ", temp->next[i]->node);
                }
            }
            printf("\n");

            // Enqueue child nodes for the next level
            for (int i = 0; i < size - (temp->level + 1); i++)
            {
                // printf("i %d tempNext %d\n",i, temp->next[i]->node);
                if (temp->next[i] != NULL)
                {
                    queue[rear++] = temp->next[i];
                }
            }
        }

        // Print a newline after processing all nodes at a level
        printf("\n");
    }
    free(queue);
}

void freeTree(bfsTree *tree)
{
    if (tree == NULL)
    {
        return; // Base case: Empty tree
    }

    // Free memory used by child nodes (recursive call)
    for (int i = 0; i < size - (tree->level + 1); i++)
    {
        if (tree->next[i] != NULL)
        {
            freeTree(tree->next[i]);
        }
    }

    // Free child node pointer array
    free(tree->next);

    // Free visited and queue arrays (if not already freed elsewhere)
    free(tree->visited);
    free(tree->queue);

    // Free the current node itself
    free(tree);
}
