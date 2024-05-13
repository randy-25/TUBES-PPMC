#ifndef BFS_H
#define BFS_H

extern int size;

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

void BFS(double **graph, char *namaKota[100], int startVertex);
int countDepthBFS(int n);
struct bfsTree *createNodeBFS(int data);
void BFSTree(struct bfsTree **tree, double **adjMatrix, int startVertex);
struct bfsTree *createTreeBFS(struct bfsTree *root, double **adjMatrix, int startVertex);
void findMinDistanceBFS(struct bfsTree *tree, int startVertex, double **adjMatrix, char *namaKota[100]);
void printTreeBFS(struct bfsTree *tree);
void freeTreeBFS(struct bfsTree *tree);

#endif