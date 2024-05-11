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

int countDepth(int n);
struct bfsTree *createNode(int data);
void BFS(struct bfsTree **tree, double **adjMatrix, int startVertex);
struct bfsTree *createTree(struct bfsTree *root, double **adjMatrix, int startVertex);
void findMinDistance(struct bfsTree *tree, int startVertex, double **adjMatrix, char *namaKota[100]);
void printTree(struct bfsTree *tree);
void freeTree(struct bfsTree *tree);

#endif