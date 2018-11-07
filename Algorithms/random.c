#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

main () {
  int i = 30, r, min = 10000, max = 0, average = 0;

  srand (time (NULL));

  while (i--) {
    r = rand ();
    if (r < min)
      min = r;
    if (r > max)
      max = r;
    average += r;

  }
  cout << average / 30 << endl;
  cout << min << endl;
  cout << max << endl;
}
