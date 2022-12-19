#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <bits/types/time_t.h>
#include <time.h>

struct ITERATION {
    int size;
    unsigned short ***tables
};

struct MOVE {
    int from;
    int to;
    unsigned short **table;
    bool win;
    int depth;
    struct MOVE *prev_move;
    struct MOVE *next_move
};

static const int FIELD_NOT_IN_THE_GAME = 7;
static const int EMPTY = 0;
static const int PUG = 1;
static const int TABLE_SIZE = 5;

unsigned short **init_table();

bool check_has_next_step(unsigned short **table);

void generate_next_moves(struct MOVE parent);

unsigned short **copy_table(unsigned short **table);

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
  time_t start, end;
  time(&start);
  int i, j = 0, k = 0;
  struct ITERATION iteration, tmp_iteration;
  unsigned short **tmp_table, **table = init_table();
  struct MOVE base;
  base.table = table;
  base.win = false;
  base.depth = 1;

  generate_next_moves(base);

  time(&end);
  printf("That's it in %2lf seconds", difftime(start, end));

}

void generate_next_moves(struct MOVE parent) {
//  printf("DEPTH: %d, FROM: %d TO: %d \n", parent.depth, parent.from, parent.to);
  unsigned short **temp_table,
          **table = parent.table;

  if (!check_has_next_step(parent.table) || parent.depth > 19) {
    return;
  }
  int i, j;
  for (i = 0; i < TABLE_SIZE; i++) {
    for (j = 0; j < TABLE_SIZE; j++) {
      if (table[i][j] == FIELD_NOT_IN_THE_GAME || table[i][j] == EMPTY) {
        continue;
      }

      if (i >= 2 && table[i - 1][j] == PUG && table[i - 2][j] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i - 1][j] = EMPTY;
        temp_table[i - 2][j] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i - 2) * TABLE_SIZE + j;
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (i >= 2 && j <= 2 && table[i - 1][j + 1] == PUG && table[i - 2][j + 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i - 1][j + 1] = EMPTY;
        temp_table[i - 2][j + 2] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i - 2) * TABLE_SIZE + (j + 2);
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (j <= 2 && table[i][j + 1] == PUG && table[i][j + 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i][j + 1] = EMPTY;
        temp_table[i][j + 2] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = i * TABLE_SIZE + (j + 2);
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (i <= 2 && j <= 2 && table[i + 1][j + 1] == PUG && table[i + 2][j + 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i + 1][j + 1] = EMPTY;
        temp_table[i + 2][j + 2] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i + 2) * TABLE_SIZE + (j + 2);
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (i <= 2 && table[i + 1][j] == PUG && table[i + 2][j] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i + 1][j] = EMPTY;
        temp_table[i + 2][j] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i + 2) * TABLE_SIZE + j;
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (i <= 2 && j >= 2 && table[i + 1][j - 1] == PUG && table[i + 2][j - 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i + 1][j - 1] = EMPTY;
        temp_table[i + 2][j - 2] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i + 2) * TABLE_SIZE + (j - 2);
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (j >= 2 && table[i][j - 1] == PUG && table[i][j - 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i][j - 1] = EMPTY;
        temp_table[i][j - 2] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = i * TABLE_SIZE + (j - 2);
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }

      if (i >= 2 && j >= 2 && table[i - 1][j - 1] == PUG && table[i - 2][j - 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i - 1][j - 1] = EMPTY;
        temp_table[i - 2][j - 2] = PUG;
        struct MOVE move;
        move.table = temp_table;
        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i - 2) * TABLE_SIZE + (j - 2);
        move.depth = parent.depth + 1;
        generate_next_moves(move);
        continue;
      }
    }
  }
}

unsigned short **copy_table(unsigned short **table) {
  unsigned short **new_table = malloc(TABLE_SIZE * sizeof(short *));

  short i, j;

  for (i = 0; i < TABLE_SIZE; i++) {
    new_table[i] = malloc(TABLE_SIZE * sizeof(short));
    for (j = 0; j < TABLE_SIZE; j++) {
      new_table[i][j] = table[i][j];
    }
  }

  return new_table;
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
