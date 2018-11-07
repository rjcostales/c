////////////////////////////////////////////////////////////////////////////////
unsigned long
htoi (char *string) {

  unsigned char hex[32] = {
    0,
    0xa, 0xb, 0xc, 0xd, 0xe, 0xf,       // 1-6
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,       // 16 - 25
    0
  };

  char *digit;

  int i = 0;

  for (i = 0; i < 32; i++)

    printf ("%02d = 0x%0x\n", i, hex[i]);

  i = 0;

  if ((strstr (string, "0x") == string)
      || (strstr (string, "0X") == string))

    digit = string + 2;

  else if ((strstr (string, "x") == string)
           || (strstr (string, "X") == string))

    digit = string + 1;

  else

    return atoi (string);

  while (*digit) {

    // how this work
    // A-F is 65-70 or 1-6 after mod 32
    // a-f is 97-102 or 1-6 after mod 32
    // 0-9 is 48-57 or 16-25 after mod 32
    if (!isxdigit (*digit))
      return 0;

    i = i * 16 + hex[*digit % 32];

    digit++;

  }

  return i;

}

int
main () {

  printf ("%d\n", htoi ("0xdeaddada"));

  return 0;

}
