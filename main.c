#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <bits/types/time_t.h>
#include <time.h>

struct MOVE {
    int from;
    int to;
    unsigned short **table;
    bool win;
    int depth;
    struct MOVE *prev_move;
    struct MOVE **next_move
};

int c = 0;

static const int FIELD_NOT_IN_THE_GAME = 7;
static const int EMPTY = 0;
static const int PEG = 1;
static const int TABLE_SIZE = 5;
static const int WIN_RESULT = 1;
static const int HAS_ANOTHER_PUG_THAT_CAN_JUMP = 2;
static const int HAS_ANOTHER_PUG_THAT_CAN_NOT_JUMP = 3;

unsigned short **init_table();

short check_has_next_step(unsigned short **table);

void generate_next_moves(struct MOVE parent, struct MOVE **winner_moves);

unsigned short **copy_table(unsigned short **table);

void free_table(unsigned short **table);

unsigned short **init_table() {
  unsigned short **table = malloc(TABLE_SIZE * sizeof(short *));

  short i, j;

  for (i = 0; i < TABLE_SIZE; i++) {
    table[i] = malloc(TABLE_SIZE * sizeof(short));
    for (j = 0; j < TABLE_SIZE; j++) {
      table[i][j] = PEG;
    }
  }

  table[0][0] = table[0][4] = table[4][0] = table[4][4] = FIELD_NOT_IN_THE_GAME;
  table[2][2] = EMPTY;
  return table;
}

int main() {
  time_t start, end;
  time(&start);
  struct MOVE base;
  struct MOVE ** win_moves = malloc(1 * sizeof (struct MOVE *));
  base.table = init_table();
  base.win = false;
  base.depth = 1;

  generate_next_moves(base, win_moves);

  time(&end);
  printf("That's it in %f seconds\n", difftime(end, start));
  int win_moves_size = sizeof win_moves / sizeof(struct MOVE *);
  printf("debug point with %d win move", win_moves_size);


}

void generate_next_moves(struct MOVE parent, struct MOVE **winner_moves) {
  c++;
//  printf("DEPTH: %d, FROM: %d TO: %d \n", parent.depth, parent.from, parent.to);
  if (parent.depth == 2) {
    printf("\n2. level\n");
  }
  unsigned short **table = parent.table;

  short check_result = check_has_next_step(table);

  if (check_result == WIN_RESULT || check_result == HAS_ANOTHER_PUG_THAT_CAN_NOT_JUMP || parent.depth > 40) {
    if (parent.depth > 40) {
      printf("out of steps\n");
    }

    if (check_result == WIN_RESULT) {
      printf("win\n");
    }
    parent.win = (check_result == WIN_RESULT);
    if (parent.win) {
      int current_size = sizeof winner_moves / sizeof(struct MOVE *);
      winner_moves = realloc(winner_moves, (current_size + 1) * sizeof (struct MOVE *));
      winner_moves[current_size + 1] = &parent;
    }
    return;
  }

  int i, j;
  for (i = 0; i < TABLE_SIZE; i++) {
    for (j = 0; j < TABLE_SIZE; j++) {
      if (table[i][j] == FIELD_NOT_IN_THE_GAME || table[i][j] == EMPTY) {
        continue;
      }

      if (i >= 2 && table[i - 1][j] == PEG && table[i - 2][j] == EMPTY) {
        struct MOVE move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i - 1][j] = EMPTY;
        move.table[i - 2][j] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i - 2) * TABLE_SIZE + j;
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, NULL);
        free_table(move.table);
        continue;
      }

      if (j <= 2 && table[i][j + 1] == PEG && table[i][j + 2] == EMPTY) {
        struct MOVE move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i][j + 1] = EMPTY;
        move.table[i][j + 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = i * TABLE_SIZE + (j + 2);
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, NULL);
        free_table(move.table);
        continue;
      }

      if (i <= 2 && table[i + 1][j] == PEG && table[i + 2][j] == EMPTY) {
        struct MOVE move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i + 1][j] = EMPTY;
        move.table[i + 2][j] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = (i + 2) * TABLE_SIZE + j;
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, NULL);
        free_table(move.table);
        continue;
      }


      if (j >= 2 && table[i][j - 1] == PEG && table[i][j - 2] == EMPTY) {
        struct MOVE move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i][j - 1] = EMPTY;
        move.table[i][j - 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j;
        move.to = i * TABLE_SIZE + (j - 2);
        move.depth = parent.depth + 1;
        move.win = false;

        generate_next_moves(move, NULL);
        free_table(move.table);
        continue;
      }
    }
  }
}

void free_table(unsigned short **table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    free(table[i]);
  }
  free(table);
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

short check_has_next_step(unsigned short **table) {
  int i, j, sum = 0;
  bool has_pug_to_jump_with = false;

  for (i = 0; i < TABLE_SIZE; i++) {
    for (j = 0; j < TABLE_SIZE; j++) {
      sum += table[i][j];

      if (!has_pug_to_jump_with) {
        if (table[i][j] == PEG && (
                (i >= 2 && table[i - 1][j] == PEG && table[i - 2][j] == EMPTY) ||
                (i >= 2 && j <= 2 && table[i - 1][j + 1] == PEG && table[i - 2][j + 2] == EMPTY) ||
                (j <= 2 && table[i][j + 1] == PEG && table[i][j + 2] == EMPTY) ||
                (i <= 2 && j <= 2 && table[i + 1][j + 1] == PEG && table[i + 2][j + 2] == EMPTY) ||
                (i <= 2 && table[i + 1][j] == PEG && table[i + 2][j] == EMPTY) ||
                (i <= 2 && j >= 2 && table[i + 1][j - 1] == PEG && table[i + 2][j - 2] == EMPTY) ||
                (j >= 2 && table[i][j - 1] == PEG && table[i][j - 2] == EMPTY) ||
                (i >= 2 && j >= 2 && table[i - 1][j - 1] == PEG && table[i - 2][j - 2] == EMPTY))
                ) {
          has_pug_to_jump_with = true;
        }
      }
    }
  }
  sum -= (table[0][0] + table[0][4] + table[4][0] + table[4][4]);

  if (sum <= 1) {
    return WIN_RESULT;
  }

  return has_pug_to_jump_with ? HAS_ANOTHER_PUG_THAT_CAN_JUMP : HAS_ANOTHER_PUG_THAT_CAN_NOT_JUMP;
}
