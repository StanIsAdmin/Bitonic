#ifndef BITONIC_H
#define BITONIC_H

#include <iostream>

#include "mpi.h"

#define MASTER_NODE 0

void printArray(int array[], int array_size);
void masterNode(int array[], int array_size);
void masterSendInts(int process_id, int value_low, int value_high);
void masterReceiveInts(int process_id, int& value_low, int& value_high);
void compare_swap(int& low, int& high);
void computeSendInt(int process_id, int value);
int computeReceiveInt();
void computeNode(int process_rank, int array_size);


#endif // BITONIC_H