#ifndef ALLOCATION_H
#define ALLOCATION_H
#include "types.h"

ListProcess loadFromFile(char*);
Node* createNode(int, int, char);
void initializeList(ListBloc*, int);
int allocAlgorithm(ListBloc*, const Process*, int);
int checkAllocation(ListBloc* list, ListProcess* listp, ListProcess* p, int choice); 
void  printListBloc(ListBloc*);
void freeListBloc(ListBloc*);
void Fusion(ListBloc*);
int checkCompatibility(ListProcess* listp, int ram_size);
int Compactage(ListBloc*);
void printAlgo(ListBloc*, ListProcess*, ListProcess*, int);    
void printProcessStatus(Process *);

#endif
