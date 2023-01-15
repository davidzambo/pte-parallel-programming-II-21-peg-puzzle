#include <stdio.h>
#include <stdbool.h>
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
static const int HAS_ANOTHER_PEG_THAT_CAN_JUMP = 2;
static const int HAS_ANOTHER_PEG_THAT_CAN_NOT_JUMP = 3;

unsigned short **init_table();

short check_has_next_step(unsigned short **table);

void generate_next_moves(Move parent, long *count_of_winner_moves, char **winner_moves, int count_of_solutions);

unsigned short **copy_table(unsigned short **table);

void free_table(unsigned short **table);

void ensure_usage_and_exit();

Params get_params(char *argv[]);

void get_base_table(unsigned short **table, FILE *input);

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
  char **winner_steps = malloc(params.count_of_solutions * sizeof(char *));
  time_t start, end;
  time(&start);
  long *count_of_winner_moves = malloc(sizeof(long long));
  *count_of_winner_moves = 0;
  FILE *output, *input;
  output = fopen(params.output_filename, "w");

  if (output == NULL) {
    printf("Error! Unable to open output file!\n");
    exit(-1);
  }

  input = fopen(params.input_filename, "r");

  if (input == NULL) {
    printf("Error! Unable to open input file!\n");
    exit(-1);
  }
  Move base;
  base.table = init_table();
  get_base_table(base.table, input);
  base.win = false;
  base.depth = 1;
  int solution_counter = 0;

  for (solution_counter = 0; solution_counter < params.count_of_solutions; solution_counter++) {
    winner_steps[solution_counter] = malloc(125 * sizeof (char));
    if (winner_steps[solution_counter] == NULL) {
      printf("\nUnable to allocate memory\n");
      exit(-1);
    }
  }

  if (output == NULL) {
    printf("\nCan't open results file\nExit\n");
    exit(1);
  }

  generate_next_moves(base, count_of_winner_moves, winner_steps, params.count_of_solutions);

  solution_counter = 0;
  while (winner_steps[solution_counter] != NULL && solution_counter < params.count_of_solutions) {
    fprintf(output, "%s", winner_steps[solution_counter++]);
  }

  time(&end);
  printf("Found %ld winner moves in %f seconds\n", *count_of_winner_moves, difftime(end, start));
}

void get_base_table(unsigned short **table, FILE *input) {
  table = malloc(TABLE_SIZE * sizeof(short *));
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while((read = getline(&line, &len, input)) != -1 ) {
    printf("line: %s\n", line);
  }
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
  if (*count_of_winner_moves >= count_of_solutions) {
    return;
  }

  checked_step_counter++;
  if (*count_of_winner_moves > 0 && *count_of_winner_moves % 10000000 == 0) {
    printf("%d million count of winner moves from %ld checked moves\n", *count_of_winner_moves / 1000000,
           checked_step_counter);
  }

  unsigned short **table = parent.table;

  short check_result = check_has_next_step(table);

  if (check_result == WIN_RESULT) {
    Move tmp_move = parent;
    while (tmp_move.prev_move != NULL) {
      char *tmp_movement_string = malloc(6 * sizeof(char));
      sprintf(tmp_movement_string, "%d>%d|", tmp_move.from, tmp_move.to);
      strcat(winner_moves[*count_of_winner_moves], tmp_movement_string);
      tmp_move = *tmp_move.prev_move;
    }
    strcat(winner_moves[*count_of_winner_moves], "\n");
  }

  if (check_result == WIN_RESULT || check_result == HAS_ANOTHER_PEG_THAT_CAN_NOT_JUMP) {
    parent.win = (check_result == WIN_RESULT);
    if (parent.win) {
      (*count_of_winner_moves)++;
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
  bool has_peg_to_jump_with = false;

  for (i = 0; i < TABLE_SIZE; i++) {
    for (j = 0; j < TABLE_SIZE; j++) {
      sum += table[i][j];

      if (!has_peg_to_jump_with) {
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
          has_peg_to_jump_with = true;
        }
      }
    }
  }
  sum -= (table[0][0] + table[0][4] + table[4][0] + table[4][4]);

  if (sum <= 1) {
    return WIN_RESULT;
  }

  return has_peg_to_jump_with ? HAS_ANOTHER_PEG_THAT_CAN_JUMP : HAS_ANOTHER_PEG_THAT_CAN_NOT_JUMP;
}
