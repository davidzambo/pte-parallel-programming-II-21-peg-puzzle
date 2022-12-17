#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

unsigned short ** init_table();

unsigned short **init_table() {
  unsigned short **table = malloc(5 * sizeof(short *));

  short i, j;

  for (i = 0; i < 5; i++) {
    table[i] = malloc(5 * sizeof(short));
    for (j = 0; j < 5; j++) {
      table[i][j] = 1;
    }
  }

  table[0][0] = table[0][4] = table[4][0] = table[4][4] = 7;
  table[2][2] = 0;
  return table;
}

int main() {
  unsigned short **table = init_table();

  printf("ez");
}