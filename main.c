#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <bits/types/time_t.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct MOVE {
    int from;
    int to;
    unsigned short **table;
    bool win;
    int depth;
    struct MOVE *prev_move;
};

struct PARAMS {
    char *input_filename;
    char *output_filename;
    int count_of_solutions;
};

typedef struct MOVE Move;
typedef struct PARAMS Params;

long checked_step_counter = 0;

static const int FIELD_NOT_IN_THE_GAME = 7;
static const int EMPTY = 0;
static const int PEG = 1;
static const int TABLE_SIZE = 5;
static const int WIN_RESULT = 1;
static const int HAS_ANOTHER_PUG_THAT_CAN_JUMP = 2;
static const int HAS_ANOTHER_PUG_THAT_CAN_NOT_JUMP = 3;

unsigned short **init_table();

short check_has_next_step(unsigned short **table);

void generate_next_moves(Move parent, long *count_of_winner_moves, char **winner_moves, int count_of_solutions);

unsigned short **copy_table(unsigned short **table);

void free_table(unsigned short **table);

void ensure_usage_and_exit();

Params get_params(char *argv[]);

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


int main(int argc, char *argv[]) {
  if (argc != 7) {
    ensure_usage_and_exit();
  }
  Params params = get_params(argv);
  char ** winner_steps = malloc(sizeof(char *));
  time_t start, end;
  time(&start);
  long *count_of_winner_moves = malloc(sizeof(unsigned long long int));
  *count_of_winner_moves = 1;
  Move base;
  base.table = init_table();
  base.win = false;
  base.depth = 1;
  FILE *output;
  output = fopen(params.output_filename, "w");

  if (output == NULL) {
    printf("\nCan't open results file\nExit\n");
    exit(1);
  }

  generate_next_moves(base, count_of_winner_moves, winner_steps, params.count_of_solutions);

  time(&end);
  printf("Found %ld winner moves in %f seconds\n", *count_of_winner_moves, difftime(end, start));
}

void ensure_usage_and_exit() {
  printf("Usage: \tpte_parallel_programming_II_21_peg_puzzle -i table.txt -o solutions.txt -c 1000\n");
  printf("\t -i\tpath to the source input file which contains the table\n");
  printf("\t\tThe file should contain an n x n table, with the following symbols:\n");
  printf("\t\tX = peg\n");
  printf("\t\tO = empty hole\n");
  printf("\t\t- = space, that cannot be used\n");
  printf("\t -o\tpath to the output data file\n");
  printf("\t\tPLAIN TEXT file with the possible solutions\n");
  printf("\t -c\tcount of the required solutions\n");
  exit(-1);
}

Params get_params(char *argv[]) {

  Params params;
  params.count_of_solutions = 0;

  for (int i = 1; i < 7; i++) {
    if (argv[i][0] != '-') {
      continue;
    }

    switch (argv[i][1]) {
      case 'c':
        params.count_of_solutions = atoi(argv[i + 1]);
        break;
      case 'i':
        params.input_filename = argv[i + 1];
        break;
      case 'o':
        params.output_filename = argv[i + 1];
        break;
      default:
        printf("Invalid input parameters!\n");
        exit(-1);
    }
  }
  return params;
}


void generate_next_moves(Move parent, long *count_of_winner_moves, char **winner_moves, int count_of_solutions) {
  if (*count_of_winner_moves > count_of_solutions) {
    return;
  }

  checked_step_counter++;
  if (*count_of_winner_moves % 10000000 == 0) {
    printf("%d million count of winner moves from %ld checked moves\n", *count_of_winner_moves / 1000000,
           checked_step_counter);
  }

  unsigned short **table = parent.table;

  short check_result = check_has_next_step(table);

  if (check_result == WIN_RESULT) {
    char *winner_move = malloc(125*sizeof (char*));
    Move tmp_move = parent;
    while (tmp_move.prev_move != NULL) {
      strcpy(winner_move, (char *) tmp_move.from);
      strcpy(winner_move, ">");
      strcpy(winner_move, (char *) tmp_move.to);
      strcpy(winner_move, "|");
      tmp_move = *tmp_move.prev_move;
    }
    winner_moves[*count_of_winner_moves] = winner_move;
  }

  if (check_result == WIN_RESULT || check_result == HAS_ANOTHER_PUG_THAT_CAN_NOT_JUMP) {
    parent.win = (check_result == WIN_RESULT);
    if (parent.win) {
      ++(*count_of_winner_moves);
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
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i - 1][j] = EMPTY;
        move.table[i - 2][j] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = (i - 2) * TABLE_SIZE + j + 1;
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }

      if (i >= 2 && j <= 2 && table[i - 1][j + 1] == PEG && table[i - 2][j + 2] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i - 1][j + 1] = EMPTY;
        move.table[i - 2][j + 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = (i - 2) * TABLE_SIZE + (j + 2 + 1);
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }

      if (j <= 2 && table[i][j + 1] == PEG && table[i][j + 2] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i][j + 1] = EMPTY;
        move.table[i][j + 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = i * TABLE_SIZE + (j + 2 + 1);
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }

      if (i <= 2 && j <= 2 && table[i + 1][j + 1] == PEG && table[i + 2][j + 2] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i + 1][j + 1] = EMPTY;
        move.table[i + 2][j + 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = (i + 2) * TABLE_SIZE + (j + 2 + 1);
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }

      if (i <= 2 && table[i + 1][j] == PEG && table[i + 2][j] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i + 1][j] = EMPTY;
        move.table[i + 2][j] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = (i + 2) * TABLE_SIZE + j + 1;
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }

      if (i <= 2 && j >= 2 && table[i + 1][j - 1] == PEG && table[i + 2][j - 2] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i + 1][j - 1] = EMPTY;
        move.table[i + 2][j - 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = (i + 2) * TABLE_SIZE + (j - 2 + 1);
        move.depth = parent.depth + 1;
        move.win = false;
        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }


      if (j >= 2 && table[i][j - 1] == PEG && table[i][j - 2] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i][j - 1] = EMPTY;
        move.table[i][j - 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = i * TABLE_SIZE + (j - 2 + 1);
        move.depth = parent.depth + 1;
        move.win = false;

        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
        free_table(move.table);
        continue;
      }

      if (i >= 2 && j >= 2 && table[i - 1][j - 1] == PEG && table[i - 2][j - 2] == EMPTY) {
        Move move;
        move.table = copy_table(table);
        move.table[i][j] = EMPTY;
        move.table[i - 1][j - 1] = EMPTY;
        move.table[i - 2][j - 2] = PEG;

        move.prev_move = &parent;
        move.from = i * TABLE_SIZE + j + 1;
        move.to = (i - 2) * TABLE_SIZE + (j - 2 + 1);
        move.depth = parent.depth + 1;
        move.win = false;

        generate_next_moves(move, count_of_winner_moves, winner_moves, count_of_solutions);
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
