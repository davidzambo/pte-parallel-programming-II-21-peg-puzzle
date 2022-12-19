#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

struct TABLE_ITERATION {
    int size;
    unsigned short ***tables
};

static const int FIELD_NOT_IN_THE_GAME = 7;
static const int EMPTY = 0;
static const int PUG = 1;
static const int TABLE_SIZE = 5;

unsigned short **init_table();

bool check_has_next_step(unsigned short **table);

struct TABLE_ITERATION generate_next_tables(unsigned short **table);

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
  int i = 0, j = 0;
  struct TABLE_ITERATION iteration;
  unsigned short **table = init_table();
  unsigned short ***tables_after_move, ***temp_table, **table_after_move;
  iteration = generate_next_tables(table);
  tables_after_move = iteration.tables;
  while (j++ < iteration.size) {
    table_after_move = tables_after_move[j];
    printf("\nThe %d table:\n", j);
    for (i = 0; i < TABLE_SIZE; i++) {
      printf("%d %d %d %d %d\n", table_after_move[i][0], table_after_move[i][1], table_after_move[i][2],
             table_after_move[i][3], table_after_move[i][4]);
    }
  }
  printf("\nThis is the end.\n");
}

struct TABLE_ITERATION generate_next_tables(unsigned short **table) {
  struct TABLE_ITERATION return_value;

  int table_counter = 0;
  unsigned short ***tables = malloc((table_counter + 1) * sizeof(short **));
  unsigned short **temp_table;
  if (!check_has_next_step(table)) {
    return return_value;
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
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (i >= 2 && j <= 2 && table[i - 1][j + 1] == PUG && table[i - 2][j + 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i - 1][j + 1] = EMPTY;
        temp_table[i - 2][j + 2] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (j <= 2 && table[i][j + 1] == PUG && table[i][j + 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i][j + 1] = EMPTY;
        temp_table[i][j + 2] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (i <= 2 && j <= 2 && table[i + 1][j + 1] == PUG && table[i + 2][j + 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i + 1][j + 1] = EMPTY;
        temp_table[i + 2][j + 2] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (i <= 2 && table[i + 1][j] == PUG && table[i + 2][j] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i + 1][j] = EMPTY;
        temp_table[i + 2][j] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (i <= 2 && j >= 2 && table[i + 1][j - 1] == PUG && table[i + 2][j - 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i + 1][j - 1] = EMPTY;
        temp_table[i + 2][j - 2] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (j >= 2 && table[i][j - 1] == PUG && table[i][j - 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i][j - 1] = EMPTY;
        temp_table[i][j - 2] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }

      if (i >= 2 && j >= 2 && table[i - 1][j - 1] == PUG && table[i - 2][j - 2] == EMPTY) {
        temp_table = copy_table(table);
        temp_table[i][j] = EMPTY;
        temp_table[i - 1][j - 1] = EMPTY;
        temp_table[i - 2][j - 2] = PUG;
        tables = realloc(tables, (++table_counter + 1) * sizeof(short **));
        tables[table_counter] = temp_table;
        continue;
      }
    }
  }
  return_value.size = table_counter;
  return_value.tables = tables;
  return return_value;
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
