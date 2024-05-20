/*
Nama : Randy Revaldo Pratama
NIM  : 13222012
Program untuk menyelesaikan TSP menggunakan algoritma DFS yaitu dengan menyelam ke dalam graph terlebih dahulu
*/

#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"

extern int size; // Global size variable

struct dfsTree *createNodeDFS(int data)
{
    struct dfsTree *newNode = (struct dfsTree *)malloc(sizeof(struct dfsTree));
    newNode->node = data;
    newNode->visited = NULL;
    newNode->queue = NULL;
    newNode->totalDistance = 0.0;
    newNode->level = 0;
    newNode->next = NULL;
    return newNode;
}

void DFS(double **graph, char *namaKota[100], int startVertex){
    dfsTree *root = NULL;
    double minDistance = 9999999999;
    int *shortestPath = (int *)malloc(sizeof(int) * size);
    root = createTreeDFS(root, graph, startVertex, &minDistance, shortestPath);
    printf("Total Distance: %lf km\n", minDistance);
    for(int i = 0; i < size; i++){
        printf("%s -> ", namaKota[shortestPath[i]]);
    }
    printf("%s\n", namaKota[startVertex]);
    freeTreeDFS(root);
}

struct dfsTree *createTreeDFS(struct dfsTree *root, double **adjMatrix, int startVertex, double *minDistance, int *shortestPath)
{
    if (root == NULL)
    { // Create the root node
        root = createNodeDFS(startVertex);
    }
    DFSTree(&root, adjMatrix, startVertex, minDistance, shortestPath, startVertex);
    return root;
}

void DFSTree(struct dfsTree **tree, double **adjMatrix, int startVertex, double *minDistance, int *shortestPath, int starterIndex)
{
    int visitedSize = (*tree)->level + 1;
    int queueSize = size - (*tree)->level - 1;
    (*tree) = realloc((*tree), sizeof(struct dfsTree) + (visitedSize + queueSize) * sizeof(int));
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
    
    
    if((*tree)->level == size - 1){
        (*tree)->totalDistance += adjMatrix[(*tree)->visited[visitedSize-1]][starterIndex];
        if((*tree)->totalDistance < (*minDistance)){
            (*minDistance) = (*tree)->totalDistance;
            for(int i = 0; i < size; i++){
                shortestPath[i] = (*tree)->visited[i];
            }
        }
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
    (*tree) = realloc((*tree), sizeof(struct dfsTree) + (queueSize) * sizeof(struct dfsTree *));
    (*tree)->next = (struct dfsTree **)malloc(sizeof(struct dfsTree *) * queueSize); // Allocate space for child pointers
    for (int i = 0; i < queueSize; i++)
    { // Process all nodes at the current level
        int currentNode = (*tree)->queue[i];
        (*tree)->next[i] = createNodeDFS(currentNode);   // Create child node
        (*tree)->next[i]->level = (*tree)->level + 1; // Set child level
        (*tree)->next[i]->visited = (int *)malloc(sizeof(int) * (visitedSize + 1));
        for (int j = 0; j < visitedSize; j++)
        {
            (*tree)->next[i]->visited[j] = (*tree)->visited[j]; // Copy visited nodes to child
        }
        if(visitedSize < size){
            DFSTree(&(*tree)->next[i], adjMatrix, currentNode, minDistance, shortestPath, starterIndex); // Recursively traverse child nodes
        }
    }
}

void freeTreeDFS(struct dfsTree *tree)
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
            freeTreeDFS(tree->next[i]);
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
