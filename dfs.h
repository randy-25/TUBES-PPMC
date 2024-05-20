/*
Nama : Randy Revaldo Pratama
NIM  : 13222012
Header File dari dfs.c untuk penghubung ke main program
*/

#ifndef DFS_H
#define DFS_H

extern int size;

typedef struct dfsTree
{
    int node;
    int *visited;
    int *queue;
    double totalDistance;
    int level;
    struct dfsTree **next; // Array of pointers to child nodes
} dfsTree;

struct dfsTree *createNodeDFS(int data);
void DFS(double **graph, char *namaKota[100], int startVertex);
struct dfsTree *createTreeDFS(struct dfsTree *root, double **adjMatrix, int startVertex, double *minDistance, int *shortestPath);
void DFSTree(struct dfsTree **tree, double **adjMatrix, int startVertex, double *minDistance, int *shortestPath, int starterIndex);
void freeTreeDFS(struct dfsTree *tree);

#endif