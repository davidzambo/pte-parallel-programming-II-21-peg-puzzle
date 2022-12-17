#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

void init_table(unsigned short **puzzle);

int main() {
  unsigned short **table = malloc(5 * sizeof(short *));
  init_table(table);

  printf("ez");
}

void init_table(unsigned short **puzzle) {
  short i, j;

  for (i = 0; i < 5; i++) {
    puzzle[i] = malloc(5 * sizeof(short));
    for (j = 0; j < 5; j++) {
      puzzle[i][j] = 1;
    }
  }

  puzzle[0][0] = puzzle[0][4] = puzzle[4][0] = puzzle[4][4] = 7;
  puzzle[2][2] = 0;
}
