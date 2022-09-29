#ifndef QUEUE_H
#define QUEUE_H
#include "types.h"

void pushProcessToQueue(ListProcess*, Process);
void popProcess(ListProcess*);
void freeListProcess(ListProcess*);
void pushLastProcess(ListProcess*, Process);

#endif