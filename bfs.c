#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"

extern int size; // Global size variable

struct bfsTree *createNode(int data)
{
    struct bfsTree *newNode = (struct bfsTree *)malloc(sizeof(struct bfsTree));
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

void *findParent(struct bfsTree **root, int level, int visited)
{
    // Use a queue for BFS traversal
    int depth = countDepth(size);
    struct bfsTree **queue = (struct bfsTree **)malloc(sizeof(struct bfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = (*root);
    int check = 0;
    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node

        while (front != rear && queue[front]->level == currentLevel)
        {
            struct bfsTree *temp = queue[front++];
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

struct bfsTree *createTree(struct bfsTree *root, double **adjMatrix, int startVertex)
{
    if (root == NULL)
    { // Create the root node
        root = createNode(startVertex);
    }
    BFS(&root, adjMatrix, startVertex); // for the first level
    // Use a queue to store nodes during level order traversal
    int depth = countDepth(size);
    struct bfsTree **queue = (struct bfsTree **)malloc(sizeof(struct bfsTree *) * depth); // Adjust size based on tree depth
    int front = 0, rear = 0;
    queue[rear++] = root;
    struct bfsTree **track = (struct bfsTree **)malloc(sizeof(struct bfsTree *) * depth);
    int count = 0;

    while (front != rear)
    {
        int currentLevel = queue[front]->level; // Get the level of the current node
        while (front != rear && queue[front]->level == currentLevel)
        {
            struct bfsTree *temp = queue[front++];
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
        struct bfsTree *tempRoot = root;
        int level = track[i]->level;
        if (level != 0)
        {
            int visited = track[i]->visited[track[i]->level - 1];
            findParent(&tempRoot, level, visited);
            if (tempRoot->next == NULL)
            {
                tempRoot = (struct bfsTree *)realloc(tempRoot, sizeof(struct bfsTree) + (size - (level)) * sizeof(struct bfsTree *));
                tempRoot->next = (struct bfsTree **)malloc(sizeof(struct bfsTree *) * (size - (level)));
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

void BFS(struct bfsTree **tree, double **adjMatrix, int startVertex)
{
    int visitedSize = (*tree)->level + 1;
    int queueSize = size - (*tree)->level - 1;
    (*tree) = realloc((*tree), sizeof(struct bfsTree) + (visitedSize + queueSize) * sizeof(int));
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
    (*tree) = realloc((*tree), sizeof(struct bfsTree) + (queueSize) * sizeof(struct bfsTree *));
    (*tree)->next = (struct bfsTree **)malloc(sizeof(struct bfsTree *) * queueSize); // Allocate space for child pointers
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

void printTree(struct bfsTree *tree)
{
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

void freeTree(struct bfsTree *tree)
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
