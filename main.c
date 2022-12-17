#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

unsigned short ** init_table();

bool check_has_next_step(unsigned short **pInt);

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
  bool has_next_step = check_has_next_step(table);
  printf("ez");
}

bool check_has_next_step(unsigned short **table) {
 int i, j, sum = 0;
 bool has_pug_to_jump_with = false;

 for (i = 0; i < 5; i++) {
   for (j = 0; j < 5; j++) {
     sum += table[i][j];

     if (!has_pug_to_jump_with) {
       if (
               (table[i][j] == 1 && i >= 2 && table[i-1][j] == 1 && table[i-2][j] == 0) ||
               (table[i][j] == 1 && i >= 2 && j >= 2 && table[i-1][j-1] == 1 && table[i-2][j-2] == 0) ||
               (table[i][j] == 1 && j >= 2 && table[i][j-1] == 1 && table[i][j-2] == 0) ||
               (table[i][j] == 1 && i <= 2 && j >= 2 && table[i-1][j+1] == 1 && table[i-2][j+2] == 0) ||
               (table[i][j] == 1 && i <= 2 && table[i+1][j] == 1 && table[i+2][j] == 0) ||
               (table[i][j] == 1 && i <= 2 && j <= 2 && table[i+1][j+1] == 1 && table[i+2][j+2] == 0) ||
               (table[i][j] == 1 && j <= 2 && table[i][j+1] == 1 && table[i][j+2] == 0) ||
               (table[i][j] == 1 && i <= 2 && j >= 2 && table[i+1][j-1] == 1 && table[i+2][j-2] == 0)
             )
       {
         has_pug_to_jump_with = true;
         break;
       }
     }
   }
 }
 sum -=  (table[0][0] + table[0][4] + table[4][0] + table[4][4]);

 if (sum == 1) {
   return false;
 }

  return has_pug_to_jump_with;
}
