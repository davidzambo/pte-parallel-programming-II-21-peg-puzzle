#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

static const int FIELD_NOT_IN_THE_GAME = 7;
static const int EMPTY = 0;
static const int PUG = 1;
static const int TABLE_SIZE = 5;

unsigned short **init_table();

bool check_has_next_step(unsigned short **pInt);

unsigned short **generateMove(unsigned short **table);

unsigned short **init_table() {
  unsigned short **table = malloc(TABLE_SIZE * sizeof(short *));

  short i, j;

  for (i = 0; i < TABLE_SIZE; i++) {
    table[i] = malloc(TABLE_SIZE * sizeof(short));
    for (j = 0; j < TABLE_SIZE; j++) {
      table[i][j] = PUG;
    }
  }

  table[0][0] = table[0][4] = table[4][0] = table[4][4] = FIELD_NOT_IN_THE_GAME;
  table[2][2] = EMPTY;
  return table;
}

int main() {
  int i = 0, j = 0;
  unsigned short **table = init_table();
  unsigned short **table_after_move = generateMove(table);
  while (i++ < 20) {
    table_after_move = generateMove(table_after_move);
    if (table_after_move == NULL) {
      printf("THAT WAS THE LAST MOVE!");
    }
  }
  for (i = 0; i < TABLE_SIZE; i++) {
    printf("%d %d %d %d %d\n", table_after_move[i][0], table_after_move[i][1], table_after_move[i][2],
           table_after_move[i][3], table_after_move[i][4]);
  }
  bool has_next_step = check_has_next_step(table);

  printf("ez");
}

unsigned short **generateMove(unsigned short **table) {
  if (!check_has_next_step(table)) {
    return NULL;
  }
  int i, j;
  for (i = 0; i < TABLE_SIZE; i++) {
    for (j = 0; j < TABLE_SIZE; j++) {
      if (table[i][j] == FIELD_NOT_IN_THE_GAME || table[i][j] == EMPTY) {
        continue;
      }

      if (i >= 2 && table[i - 1][j] == PUG && table[i - 2][j] == EMPTY) {
        table[i][j] = EMPTY;
        table[i - 1][j] = EMPTY;
        table[i - 2][j] = PUG;
        return table;
      }

      if (i >= 2 && j <= 2 && table[i - 1][j + 1] == PUG && table[i - 2][j + 2] == EMPTY) {
        table[i][j] = EMPTY;
        table[i - 1][j + 1] = EMPTY;
        table[i - 2][j + 2] = PUG;
        return table;
      }

      if (j <= 2 && table[i][j + 1] == PUG && table[i][j + 2] == EMPTY) {
        table[i][j] = EMPTY;
        table[i][j + 1] = EMPTY;
        table[i][j + 2] = PUG;
        return table;
      }

      if (i <= 2 && j <= 2 && table[i + 1][j + 1] == PUG && table[i + 2][j + 2] == EMPTY) {
        table[i][j] = EMPTY;
        table[i + 1][j + 1] = EMPTY;
        table[i + 2][j + 2] = PUG;
        return table;
      }

      if (i <= 2 && table[i + 1][j] == PUG && table[i + 2][j] == EMPTY) {
        table[i][j] = EMPTY;
        table[i + 1][j] = EMPTY;
        table[i + 2][j] = PUG;
        return table;
      }

      if (i <= 2 && j >= 2 && table[i + 1][j - 1] == PUG && table[i + 2][j - 2] == EMPTY) {
        table[i][j] = EMPTY;
        table[i + 1][j - 1] = EMPTY;
        table[i + 2][j - 2] = PUG;
        return table;
      }

      if (j <= 2 && table[i][j - 1] == 1 && table[i][j - 2] == 0) {
        table[i][j] = EMPTY;
        table[i][j - 1] = EMPTY;
        table[i][j - 2] = PUG;
        return table;
      }

      if (i >= 2 && j >= 2 && table[i - 1][j - 1] == 1 && table[i - 2][j - 2] == 0) {
        table[i][j] = EMPTY;
        table[i - 1][j - 1] = EMPTY;
        table[i - 2][j - 2] = PUG;
        return table;
      }
    }
  }
  return NULL;
}

bool check_has_next_step(unsigned short **table) {
  int i, j, sum = 0;
  bool has_pug_to_jump_with = false;

  for (i = 0; i < TABLE_SIZE; i++) {
    for (j = 0; j < TABLE_SIZE; j++) {
      sum += table[i][j];

      if (!has_pug_to_jump_with) {
        if (table[i][j] == PUG && (
              (i >= 2 && table[i - 1][j] == PUG && table[i - 2][j] == EMPTY) ||
              (i >= 2 && j <= 2 && table[i - 1][j + 1] == PUG && table[i - 2][j + 2] == EMPTY) ||
              (j <= 2 && table[i][j + 1] == PUG && table[i][j + 2] == EMPTY) ||
              (i <= 2 && j <= 2 && table[i + 1][j + 1] == PUG && table[i + 2][j + 2] == EMPTY) ||
              (i <= 2 && table[i + 1][j] == PUG && table[i + 2][j] == EMPTY) ||
              (i <= 2 && j >= 2 && table[i + 1][j - 1] == PUG && table[i + 2][j - 2] == EMPTY) ||
              (j >= 2 && table[i][j - 1] == PUG && table[i][j - 2] == EMPTY) ||
              (i >= 2 && j >= 2 && table[i - 1][j - 1] == PUG && table[i - 2][j - 2] == EMPTY))
        ) {
          has_pug_to_jump_with = true;
          break;
        }
      }
    }
  }
  sum -= (table[0][0] + table[0][4] + table[4][0] + table[4][4]);

  if (sum <= 1) {
    return false;
  }

  return has_pug_to_jump_with;
}
