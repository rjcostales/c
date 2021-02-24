#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define LOOP 1000
#define SIZE 10000

#define ELAPSE_TIME(X) ((float) (X) / (float) CLOCKS_PER_SEC)

// __attribute__((always_inline))
inline static float square(float n) { return n * n; }

int main(int argc, char *argv[])
{
	float reals[SIZE];
   float squares[SIZE];
	float result = 0.0;
	time_t t;
	srand((unsigned) time(&t));

	double dummy, dummy2;
	clock_t start, stop;

	printf("%s\t%'dx%'d\n", argv[0], LOOP, SIZE);

	// random array of floats 1 & 10
	for (int i = 0; i < SIZE; i++)
		reals[i] = 1 + 9 * (float) rand() / (float) RAND_MAX;

	for (int i = 0; i < SIZE; i++) {
		float f = reals[i];
		squares[i] = f * f;
	}

	double tf = 0;
	double ts = 0;

	for (int i = 0; i < SIZE; i++) {
		float f = reals[i];
		float s = squares[i];

		tf += f;
		ts += s;
		// printf("%d\t%0.4f\t%0.4f\n", i+1, f, s);
	}
	printf("%0.4f\t%0.4f\n", tf, ts);

#undef RUN
#ifdef RUN
	// test multiplication
	start = clock();
	dummy = 0;
	for (int t = 0; t < LOOP; t++) {
		result = 0;
		for (int i = 0; i < SIZE; i++) {
			float f = reals[i];
			result += f * f;
		}
		dummy += result;
	}
	stop = clock();
	printf("f * f\t\t%0.4f\t%0.4f\n", ELAPSE_TIME(stop - start), dummy);

	// test function square
	start = clock();
	dummy = 0;
	for (int t = 0; t < LOOP; t++) {
		result = 0;
		for (int i = 0; i < SIZE; i++) {
			float f = reals[i];
			result += square(f);
		}
		dummy += result;
	}
	stop = clock();
	printf("square(f)\t%0.4f\t%0.4f\n", ELAPSE_TIME(stop - start), dummy);

	// test pow()
	start = clock();
	dummy2 = 0;
	for (int t = 0; t < LOOP; t++) {
		result = 0;
		for (int i = 0; i < SIZE; i++) {
			float f = reals[i];
			result += pow(f, 2);
		}
		dummy2 += result;
	}
	stop = clock();
	printf("pow(f, 2)\t%0.4f\t%0.4f\n", ELAPSE_TIME(stop - start), dummy2);
#endif

	if ( dummy == dummy2 )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;

}
  