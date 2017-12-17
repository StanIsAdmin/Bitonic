#include <iostream>
#include <math.h>

#include "mpi.h"

#define MASTER_NODE 0
#define SORT_FIRST true
#define VERBOSE false//(process_rank == 0) ? true : false


void printArray(int array[], int array_size);
void masterNode(int array[], int array_size);
void masterCall(int array[], int block_size);
void masterSendInts(int target_rank, int value_low, int value_high);
void masterReceiveInts(int source_rank, int& value_low, int& value_high);
void compare_swap(int& low, int& high, bool xor_value);
void computeSendInt(int target_rank, int value);
int computeReceiveInt(int source_rank);
void computeNode(int array_size);
void computeCall(int max_depth);

// Global values
int process_rank, process_count;

int main(int argc, char * argv[]) {
	// Initialization
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	if (VERBOSE) {
		std::cout << "Process " << process_rank << " out of " << process_count << " started" << std::endl;
	}
	
	// Wait for all processes to initiate
	MPI_Barrier(MPI_COMM_WORLD);
	
	// We assume to have one process for two array items + one master process
	int array_size = (process_count - 1) * 2;
	
	// We verify that the array size is a power of two
	//assert(array_size == pow2(static_cast<int>(log2(array_size))));
    
    if (array_size==16) {
        if (process_rank==MASTER_NODE) {
            // master node portion
            int array[] = {14, 16, 15, 11, 9, 8, 7, 5, 4, 2, 1, 3, 6, 10, 12, 13};
			//int array[] = {12, 14, 3, 2, 6, 11, 9, 10, 1, 7, 8, 15, 5, 4, 13, 0};
            masterNode(array, array_size);
        } else {
            // computing node portion
            computeNode(array_size);
        }
    } else if (array_size==8) { 
		if (process_rank==MASTER_NODE) {
            // master node portion
            //int array[] = {4, 3, 2, 1, 5, 6, 7, 8};
			int array[] = {7, 6, 5, 4, 3, 2, 1, 0};
            masterNode(array, array_size);
        } else {
            // computing node portion
            computeNode(array_size);
        }
	}else {
		return 1;
	}
    
    MPI_Finalize();
    return 0;
}


void printArray(int array[], int array_size) {
	// Print Sorting Results
	for (int i = 0; i < array_size; i++) {
		std::cout << array[i] << " ";
	}
	std::cout << std::endl;
}


void masterNode(int array[], int array_size) {
	int max_block_size = array_size/2;
	
	// BITONIC SORT
	if (SORT_FIRST) {
		std::cout << "Arbitrary array : " << std::endl;
		printArray(array, array_size);
		
		for (int block_size=2; block_size<=max_block_size; block_size*=2) {
			masterCall(array, block_size);
			std::cout << "Temp Array : " << std::endl;
			printArray(array, array_size);
		}
	}
	
	std::cout << "Bitonic array : " << std::endl;
	printArray(array, array_size);
	
	// BITONIC MERGE
	masterCall(array, max_block_size*2);

	std::cout << "Sorted array : " << std::endl;
	printArray(array, array_size);
}

void masterCall(int array[], int block_size) {
	int half_block = block_size/2;
	
	// Share array data with processes
	for (int compute_id = 0; compute_id < process_count-1; compute_id++) {
		int offset = (compute_id/half_block) * block_size;
		offset += compute_id%half_block;
		masterSendInts(compute_id+1, array[offset], array[offset+half_block]);
	}

	// Receive merged values from processes
	for (int compute_id = 0; compute_id < process_count-1; compute_id++) {
		masterReceiveInts(compute_id+1, array[compute_id*2], array[compute_id*2+1]);
	}
}


void masterSendInts(int target_rank, int value_low, int value_high) {
	if (VERBOSE) {
		std::cout << "[" << process_rank << "] - ";
		std::cout << "Sending values to " << target_rank << ": " << value_low << ", " << value_high << std::endl;
	}
	int values[] = {value_low, value_high};
	MPI_Send(
		&values,
		2,
		MPI_INT,		// type
		target_rank,
		0, 				// tag
		MPI_COMM_WORLD
	);
}


void masterReceiveInts(int source_rank, int& value_low, int& value_high) {
	int values[2];
	MPI_Recv(
		&values,
		2,
		MPI_INT,		// type
		source_rank,
		0,				// tag
		MPI_COMM_WORLD,
		MPI_STATUS_IGNORE
	);
	value_low = values[0];
	value_high = values[1];
	if (VERBOSE) {
		std::cout << "[" << process_rank << "] - ";
		std::cout << "Received values from " << source_rank << ": " << value_low << ", " << value_high << std::endl;
	}
}


void compare_swap(int& low, int& high, bool xor_value) {
	if ((low > high) ^ xor_value) std::swap(low, high);
}

void computeSendInt(int target_rank, int value) {
	if (VERBOSE) {
		std::cout << "[" << process_rank << "] - ";
		std::cout << "Sending value to " << target_rank << ": " << value << std::endl;
	}
	MPI_Send(
		&value,
		1,
		MPI_INT,		// type
		target_rank,
		0, 				// tag
		MPI_COMM_WORLD
	);
}

int computeReceiveInt(int source_rank) {
	int value;
	MPI_Recv(
		&value,
		1,
		MPI_INT,		// type
		source_rank,
		0,				// tag
		MPI_COMM_WORLD,
		MPI_STATUS_IGNORE
	);
	if (VERBOSE) {
		std::cout << "[" << process_rank << "] - ";
		std::cout << "Received value from " << source_rank << ": " << value << std::endl;
	}
	return value;
}

void computeNode(int array_size) {
	int max_depth = log2(array_size)-1;
	
	// BITONIC SORT
	if (SORT_FIRST) {
		for (int depth=0; depth<max_depth; depth++) {
			computeCall(depth);
		}
	}
	
	// BITONIC MERGE
	computeCall(max_depth);
}
	
void computeCall(int max_depth) {
	// Computing indexing starts at 0
	int compute_id = process_rank - 1;
	
	// Receive values to merge from master
	int value_low, value_high;
	masterReceiveInts(MASTER_NODE, value_low, value_high);
	
	// True if the comparison is inversed
	bool opposite_order = ((compute_id & ( 1 << max_depth )) >> max_depth);
	
	// BITONIC MERGE
	for (int depth = max_depth-1; depth >= 0; depth--) {
		compare_swap(value_low, value_high, opposite_order);
		
		int paired_rank = (compute_id ^ (1 << depth)) + 1;
		bool merge_down = ((compute_id & ( 1 << depth )) >> depth);
		
		if (! merge_down) {
			computeSendInt(paired_rank, value_high);
			value_high = computeReceiveInt(paired_rank);
		} else {
			computeSendInt(paired_rank, value_low);
			value_low = computeReceiveInt(paired_rank);
		}
	}
	
	// Last swap does not require to communicate with compute processes
	compare_swap(value_low, value_high, opposite_order);
	
	// Send back sorted values
	masterSendInts(MASTER_NODE, value_low, value_high);
}
