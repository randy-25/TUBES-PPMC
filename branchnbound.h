#ifndef BNB_H
#define BNB_H

extern int size;
double firstMin(double **graph, int vertex);
double secondMin(double **graph, int vertex);
void copytoFinal(int curr_path[], int final_path[]);
void BranchnBoundRec(double **graph, double *final_res, double curr_bound, double curr_weight, int level, int curr_path[], int final_path[], int visited[], int startvertex);
void BranchnBound(double **graph, int startvertex, char *cityName[100]);

#endif