#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){
	int thread_id;

	#pragma omp parallel private(thread_id)
	{
		printf("Hello from process: %d\n", omp_get_thread_num());
	}
}
