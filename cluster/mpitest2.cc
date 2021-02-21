#include <cstdlib>
#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	int numprocessors, rank, namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

	int number;

	if (rank == 0) {
		number = -4;
		for (int i = 1; i < numprocessors; i++)
			MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	} else {
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "I am " << processor_name 
		     << " and got " << number 
		     << " from node 0." << endl;
	}

	MPI_Finalize();
}
