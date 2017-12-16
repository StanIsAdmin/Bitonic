#include "bitonic.hpp"


int main(int argc, char * argv[]) {
	int process_rank, process_count;

	// Initialization
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	
	// We assume to have one process for two array items + one master process
	int array_size = (process_count - 1) * 2;
	
	// We verify that the array size is a power of two
	//assert(array_size == pow2(static_cast<int>(log2(array_size))));
    
    if (array_size==16) {
        if (process_rank==MASTER_NODE) {
            // master node portion
            int array[] = {14, 16, 15, 11, 9, 8, 7, 5, 4, 2, 1, 3, 6, 10, 12, 13};
            masterNode(array, array_size);
        } else {
            // computing node portion
            computeNode(process_rank, array_size);
        }
    } else {
		return 1;
	}
    
    MPI_Finalize();
    return 0;
}


void printArray(int array[], int array_size) {
	// Print Sorting Results
	for (int i = 0; i < array_size; i++) {
		std::cout << array[i];
	}
	std::cout << std::endl;
}


void masterNode(int array[], int array_size) {
	std::cout << "Unsorted array : " << std::endl;
	printArray(array, array_size);
	
	// Wait for all processes to instantiate
	MPI_Barrier(MPI_COMM_WORLD);
	
	// Share array data with processes
	int block_size = array_size/2;
    for (int process_id = 1; process_id<=array_size; process_id++) {
		masterSendInts(process_id, array[process_id], array[process_id+block_size]);
	}

	// Blocks until all processes have finished sorting
	MPI_Barrier(MPI_COMM_WORLD);
	
	// 

	std::cout << "Sorted array : " << std::endl;
	printArray(array, array_size);
}


void masterSendInts(int process_id, int value_low, int value_high) {
	int values[] = {value_low, value_high};
	MPI_Send(
		&values,
		2,
		MPI_INT,		// type
		process_id,
		0, 				// tag
		MPI_COMM_WORLD
	);
}


void masterReceiveInts(int process_id, int& value_low, int& value_high) {
	int values[2];
	MPI_Recv(
		&values,
		2,
		MPI_INT,		// type
		process_id,
		0,				// tag
		MPI_COMM_WORLD,
		MPI_STATUS_IGNORE
	);
	value_low = values[0];
	value_high = values[1];
}

void compare_swap(int& low, int& high) {
	if (low > high) std::swap(low, high);
}

void computeSendInt(int process_id, int value) {
	MPI_Send(
		&value,
		1,
		MPI_INT,		// type
		process_id,
		0, 				// tag
		MPI_COMM_WORLD
	);
}

int computeReceiveInt() {
	int value;
	MPI_Recv(
		&value,
		1,
		MPI_INT,		// type
		MPI_ANY_SOURCE,
		0,				// tag
		MPI_COMM_WORLD,
		MPI_STATUS_IGNORE
	);
	return value;
}

void computeNode(int process_rank, int array_size) {
	// Computing indexing starts at 0
	int compute_id = process_rank - 1;
	
	// Receive two initial values
	int value_low, value_high;
	masterReceiveInts(MASTER_NODE, value_low, value_high);
	
	int block_size = array_size/2;
	while (block_size >= 1) {
		compare_swap(value_low, value_high);
		
		block_size /= 2;
		int target_rank = (compute_id + 1 + block_size) % (block_size*2);
		if (compute_id < block_size) {
			computeSendInt(target_rank, value_high);
			value_high = computeReceiveInt();
		} else {
			computeSendInt(target_rank, value_low);
			value_low = computeReceiveInt();
		}
	}
}
