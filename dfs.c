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
    root = createTreeDFS(root, graph, startVertex);
    findMinDistanceDFS(root, startVertex, graph, namaKota);
    freeTreeDFS(root);
}

struct dfsTree *createTreeDFS(struct dfsTree *root, double **adjMatrix, int startVertex)
{
    if (root == NULL)
    { // Create the root node
        root = createNodeDFS(startVertex);
    }
    DFSTree(&root, adjMatrix, startVertex);
    return root;
}

void DFSTree(struct dfsTree **tree, double **adjMatrix, int startVertex)
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
    // printf("Node: %d, Level: %d\n", (*tree)->node, (*tree)->level);
    // for(int i = 0; i < visitedSize; i++){
    //     printf("Visited: %d\n", (*tree)->visited[i]);
    // }
    // printf("\n");
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
            DFSTree(&(*tree)->next[i], adjMatrix, currentNode); // Recursively traverse child nodes
        }
    }
}

void findMinDistanceDFS(struct dfsTree *tree, int startVertex, double **adjMatrix, char *namaKota[100])
{
    double minDistance = 9999999.99;
    int *shortestPath = (int *)malloc(sizeof(int) * (size + 1));
    if (tree == NULL)
    {
        return; // Base case: Empty tree
    }
    int depth = countDepthDFS(size);
    // Use a queue to store nodes during level order traversal
    struct dfsTree **queue = (struct dfsTree **)malloc(sizeof(struct dfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = tree;

    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node

        // Print all nodes at the current level
        while (front != rear && queue[front]->level == currentLevel)
        {
            struct dfsTree *temp = queue[front++];
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

int countDepthDFS(int maxSize)
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

void printTreeDFS(struct dfsTree *tree)
{
    if (tree == NULL)
    {
        return; // Base case: Empty tree
    }
    int depth = countDepthDFS(size);
    // Use a queue to store nodes during level order traversal
    struct dfsTree **queue = (struct dfsTree **)malloc(sizeof(struct dfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = tree;

    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node

        // Print all nodes at the current level
        while (front != rear && queue[front]->level == currentLevel)
        {
            struct dfsTree *temp = queue[front++];
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
