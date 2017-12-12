#include <iostream>
#include <string>
#include <mpi.h>

#include "BitonicSort.hpp"

void masterNode(int list[], int size);
void computeNode();

int main(int argc, char *argv[]) {
    int rank, nb_instance;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_instance);
    MPI_Status status;
    
    int cnodes=nb_instance-1;
    int n=cnodes*2;
    
    if (n==16) {
        if (rank==0) {
            // master node portion
            int list[] = {14, 16, 15, 11, 9, 8, 7, 5, 4, 2, 1, 3, 6, 10, 12, 13};
            masterNode(list, n);
        } else {
            // computing node portion
            computeNode();
        }
    }
    
    MPI_Finalize();
    return 0;
}

void masterNode(int list[], int size) {
    std::cout << "master node" << std::endl;
}

void computeNode() {
    std::cout << "compute node" << std::endl;
}
