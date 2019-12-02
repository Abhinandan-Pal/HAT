#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

unsigned short size_board = 10;
unsigned short size_hole = 5;

short last_moves_x[2][5] = {{-100, -100, -100, -100, -100},
                            {-100, -100, -100, -100, -100}};
short last_moves_y[2][5] = {{-100, -100, -100, -100, -100},
                            {-100, -100, -100, -100, -100}};
char *board;

int min(int arg_count, ...) {
  int i;
  int min, a;

  va_list ap;

  va_start(ap, arg_count);

  min = va_arg(ap, int);

  for (i = 2; i <= arg_count; i++)
    if ((a = va_arg(ap, int)) < min)
      min = a;

  va_end(ap);

  return min;
}

void board_reset() {
  for (int y = 0; y < 2 * size_board; ++y) {
    for (int x = 0; x < 2 * size_board; ++x) {
      board[2 * size_board * y + x] = '.';
    }
  }

  for (int y = size_board - size_hole; y < size_board + size_hole; ++y) {
    for (int x = size_board - size_hole; x < size_board + size_hole; ++x) {
      board[2 * size_board * y + x] = ' ';
    }
  }
}

void board_draw() {
  printf("  ");
  for (int x = 0; x < 2 * size_board; ++x) {
    printf(" %c", 'A' + x);
  }
  putchar('\n');

  for (int y = 0; y < 2 * size_board; ++y) {
    printf("%2u ", y + 1);
    for (int x = 0; x < 2 * size_board; ++x) {
      putchar(board[2 * size_board * y + x]);
      putchar(' ');
    }
    printf("%2u\n", y + 1);
  }

  printf("  ");
  for (int x = 0; x < 2 * size_board; ++x) {
    printf(" %c", 'A' + x);
  }
  putchar('\n');
}

int near(int x1, int x2, int y1, int y2) {
  int x = abs(x1 - x2);
  int y = abs(y1 - y2);

  return (x == 1 && y == 0) || (x == 0 && y == 1);
}

int valid(char player, int target_x, int target_y) {
  if (last_moves_x[player - 'A'][4] < 0 && last_moves_y[player - 'A'][4] < 0) {
    return 1;
  }

  return near(last_moves_x[player - 'A'][4], target_x,
              last_moves_y[player - 'A'][4], target_y) ||
         near(last_moves_x[player - 'A'][3], target_x,
              last_moves_y[player - 'A'][3], target_y) ||
         near(last_moves_x[player - 'A'][2], target_x,
              last_moves_y[player - 'A'][2], target_y);
}

int final(char player, int target_x, int target_y) {

  char player2 = (player == 'A') ? 'B' : 'A';

  if (board[target_y * 2 * size_board + target_x] == player2)
    return 1; // has eaten the head.
  if (board[target_y * 2 * size_board + target_x] == tolower(player2))
    return 2; // has eaten the tail.
  return 0;

  /*int j = min(3, (last_moves[player2 - 'A'][0] - curr_loc),
              (last_moves[player2 - 'A'][1] - curr_loc),
              (last_moves[player2 - 'A'][2] - curr_loc));

  if (j == 0)
    return 1; // has eaten the head.

  j = min(2, (last_moves[player2 - 'A'][3] - curr_loc),
          (last_moves[player2 - 'A'][4] - curr_loc));

  if (j == 0)
    return 2;

  return 0;   // nothing important happened.*/
}
int main(int argc, char const *argv[]) {
  if (argc > 1) {
    size_board = atoi(argv[1]);
    if (argc > 2) {
      size_hole = atoi(argv[2]);
    }
  }

  board = (char *)malloc(4 * size_board * size_board * sizeof(char));
  board_reset();

  char player = 'A';
  while (1) {
    board_draw();
    char target_x;
    short target_y;
    while (1) {
      printf("Player %c > ", player);

      if (scanf("%hu%c", &target_y, &target_x) != 2)
        continue;

      target_x = toupper(target_x) - 'A';
      if (target_x < 0 || target_x >= size_board * 2)
        continue;

      target_y--;
      if (target_y < 0 || target_y >= size_board * 2)
        continue;

      if (valid(player, target_x, target_y) == 1)
        break;
      printf("Invalid input\n");
    }
    if (final(player, target_x, target_y) == 1) {
      printf("******Player %c has WON******\n", player);
      break;
    }
    if (final(player, target_x, target_y) == 2) {
      printf("******Player %c has LOST******\n", player);
      break;
    }

    board[target_y * 2 * size_board + target_x] = player;

    if (last_moves_y[player - 'A'][0] >= 0 &&
        last_moves_x[player - 'A'][0] >= 0) {
      board[last_moves_y[player - 'A'][0] * 2 * size_board +
            last_moves_x[player - 'A'][0]] = '.';
    }
    if (last_moves_y[player - 'A'][2] >= 0 &&
        last_moves_x[player - 'A'][2] >= 0) {
      board[last_moves_y[player - 'A'][2] * 2 * size_board +
            last_moves_x[player - 'A'][2]] =
          tolower(board[last_moves_y[player - 'A'][2] * 2 * size_board +
                        last_moves_x[player - 'A'][2]]);
    }
    last_moves_x[player - 'A'][0] = last_moves_x[player - 'A'][1];
    last_moves_x[player - 'A'][1] = last_moves_x[player - 'A'][2];
    last_moves_x[player - 'A'][2] = last_moves_x[player - 'A'][3];
    last_moves_x[player - 'A'][3] = last_moves_x[player - 'A'][4];
    last_moves_x[player - 'A'][4] = target_x;

    tolower(board[last_moves_y[player - 'A'][2]]);
    last_moves_y[player - 'A'][0] = last_moves_y[player - 'A'][1];
    last_moves_y[player - 'A'][1] = last_moves_y[player - 'A'][2];
    last_moves_y[player - 'A'][2] = last_moves_y[player - 'A'][3];
    last_moves_y[player - 'A'][3] = last_moves_y[player - 'A'][4];
    last_moves_y[player - 'A'][4] = target_y;

    player = (player == 'A') ? 'B' : 'A';
  }

  free(board);

  return 0;
}
