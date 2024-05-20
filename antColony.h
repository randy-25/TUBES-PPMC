/*
Nama : Shalawatul Shafa
NIM : 13222007
Deskripsi Program: algoritma ant colony optimization yang dapat menyelesaikan permasalahan Travelling Salesman Problem.algoritma ini menggunakan 
                  konsep semut untuk mencari rute terpendek dalam mencari makanan,rute yang dipilih menggunakan perbandingan probabilitas pengaruh penanda
                  /feromon.
*/
#ifndef ANT_COLONY_H
#define ANT_COLONY_H

extern int size;

void antCol(double **graph, int startingIndex, char *namaKota[100]);
void updatePenanda(double **Penanda, double **deltaPenanda, int size);
void calculateDeltaPenanda(double **deltaPenanda, double *antDistances);
void antColonyOptimization(double **graph, double **Penanda, double **deltaPenanda, int *bestTour, double *bestTourLength);
void findBestTourIndices(int *bestTour, int *bestTourIndices, char *namaKota[100], int size);

#endif
