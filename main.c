#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

unsigned short size_board = 10;
unsigned short size_hole = 5;

short last_moves[2][5] = {{-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}};

char *board;

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
    printf("Player %c > ", player);

    char target_x;
    short target_y;
    if (scanf("%hu%c", &target_y, &target_x) != 2)
      continue;

    target_x = toupper(target_x) - 'A';
    if (target_x < 0 || target_x >= size_board * 2)
      continue;

    target_y--;
    if (target_y < 0 || target_y >= size_board * 2)
      continue;

    board[target_y * 2 * size_board + target_x] = player;

    board[last_moves[player - 'A'][0]] = '.';
    board[last_moves[player - 'A'][2]] =
        tolower(board[last_moves[player - 'A'][2]]);
    last_moves[player - 'A'][0] = last_moves[player - 'A'][1];
    last_moves[player - 'A'][1] = last_moves[player - 'A'][2];
    last_moves[player - 'A'][2] = last_moves[player - 'A'][3];
    last_moves[player - 'A'][3] = last_moves[player - 'A'][4];
    last_moves[player - 'A'][4] = target_y * 2 * size_board + target_x;

    player = (player == 'A') ? 'B' : 'A';
  }

  free(board);

  return 0;
}
