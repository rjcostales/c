/* binary search example  */
void main()
{
  int     high, lo, mid, found = false;
  int     target = 367;
  int     a[10];

  /* seed the array */
  a[0] = 3;
  a[1] = 6;
  a[2] = 19;
  a[3] = 43;
  a[4] = 58;
  a[5] = 112;
  a[6] = 367;
  a[7] = 390;
  a[8] = 427;
  a[9] = 471;

  lo = 0;
  high = 9;
  mid = (high + lo) / 2;

  while (lo < high) {

    if (target <= a[mid])
      high = mid;
    else
      lo = mid + 1;

    mid = (high + lo) / 2;
  }

  if (a[mid] == target)
    found = true;

  if (found)
    printf("found target: %d\n", target);
  else
    printf("target %d not found\n", target);

}
