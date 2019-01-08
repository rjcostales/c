#include <iostream.h>
#include <stdio.h>
#include <curses.h>

WINDOW *w;

void
poll () {
  char c;

  c = wgetch (w);

  if (c == 'p')
    mvprintw (10, 10, "p pressed");

}

int
main () {

  w = initscr ();
  nonl ();
  nodelay (w, 1);
  noecho ();

  while (1) {
    poll ();

    cout << "this is a test\n";
  }

  endwin ();
}
