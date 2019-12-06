
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE_HEAD 4
#define SIZE_TAIL 5
#define SIZE_SNAKE (SIZE_TAIL + SIZE_HEAD)

unsigned short size_board = 10;
unsigned short size_hole = 5;

short last_moves_x[2][SIZE_SNAKE];
short last_moves_y[2][SIZE_SNAKE];
char *board;
void fill_moves() {
  for (int i = 0; i < SIZE_SNAKE; i++) {
    last_moves_x[0][i] = -100;
    last_moves_x[1][i] = -100;
    last_moves_y[0][i] = -100;
    last_moves_y[1][i] = -100;
  }
}
short choices_x[SIZE_HEAD * 4];
short choices_y[SIZE_HEAD * 4];
short nchoices = 0;

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
  printf("  ");
  for (int x = 0; x < 2 * size_board; ++x) {
    printf("%2u", x);
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

// takes the current co-ordiante and teleports it to required point
void teleport(short int *x_pos, short int *y_pos) {
  if (board[*y_pos * size_board * 2 + *x_pos] == ' ') {

    if (*x_pos == (size_board - size_hole)) { // Present at Left
      *x_pos += 2 * size_hole;
      short int j = 2 * size_hole - (*y_pos - (size_board - size_hole)) - 1;
      *y_pos += j;
      return;
    }

    if (*x_pos == (size_board + size_hole - 1)) { // Present at Right
      *x_pos -= (2 * size_hole);
      short int j = 2 * size_hole - (*y_pos - (size_board - size_hole)) - 1;
      *y_pos += j;
      return;
    }

    if (*y_pos == (size_board - size_hole)) { // Present at Top
      *y_pos += 2 * size_hole;
      short int j = 2 * size_hole - (*x_pos - (size_board - size_hole)) - 1;
      *x_pos += j;
      return;
    }

    if (*y_pos == (size_board + size_hole - 1)) { // Present at Bottom
      *y_pos -= (2 * size_hole);
      short int j = 2 * size_hole - (*x_pos - (size_board - size_hole)) - 1;
      *x_pos += j;
      return;
    }
  }
}

void possible(char player) {
  // Generates all valid move list for current player

  nchoices = 0;
  for (int i = SIZE_SNAKE - 1; i >= SIZE_TAIL; i--) {
    int x_pos = last_moves_x[player - 'A'][i];
    int y_pos = last_moves_y[player - 'A'][i];

    if (x_pos < 0)
      break;

    choices_x[nchoices] = (x_pos + 1) % (2 * size_board);
    choices_y[nchoices] = y_pos;
    teleport(&choices_x[nchoices], &choices_y[nchoices]);
    nchoices++;

    choices_x[nchoices] =
        ((x_pos - 1) < 0) ? 2 * size_board + x_pos - 1 : x_pos - 1;
    choices_y[nchoices] = y_pos;
    teleport(&choices_x[nchoices], &choices_y[nchoices]);
    nchoices++;

    choices_x[nchoices] = x_pos;
    choices_y[nchoices] =
        ((y_pos - 1) < 0) ? 2 * size_board + y_pos - 1 : y_pos - 1;
    teleport(&choices_x[nchoices], &choices_y[nchoices]);
    nchoices++;

    choices_x[nchoices] = x_pos;
    choices_y[nchoices] = (y_pos + 1) % (2 * size_board);
    teleport(&choices_x[nchoices], &choices_y[nchoices]);
    nchoices++;
  }
}

void moves_print() {
  printf("Your Valid move list : \n");
  for (int i = 0; i < nchoices; i++) {
    if (i % 4 == 0)
      printf("\n");
    printf("%d%c ", choices_y[i] + 1, choices_x[i] + 'A');
  }
  printf("\n");
}
int valid(char player, short target_x, short target_y) {
  char otherplayer = (player == 'A') ? 'B' : 'A';

  char current = board[2 * size_board * target_y + target_x];
  if (current != '.' && current != otherplayer &&
      current != tolower(otherplayer))
    return 0;

  if (last_moves_x[player - 'A'][SIZE_SNAKE - 1] < 0 &&
      last_moves_y[player - 'A'][SIZE_SNAKE - 1] < 0) {
    return 1; // no moves yet, all moves valid
  }

  for (int i = 0; i < nchoices; ++i) {
    if (choices_y[i] == target_y && choices_x[i] == target_x) {
      return 1;
    }
  }

  return 0;
}

int final(char player, int target_x, int target_y) {

  char player2 = (player == 'A') ? 'B' : 'A';

  if (board[target_y * 2 * size_board + target_x] == player2)
    return 1; // has eaten the head.
  if (board[target_y * 2 * size_board + target_x] == tolower(player2))
    return 2; // has eaten the tail.
  return 0;
}
void info() {
  printf("\n\n 88         88     888888888     888888888888\n");
  printf(" 88         88   88         88        88\n");
  printf(" 88         88   88         88        88\n");
  printf(" 88         88   88         88        88\n");
  printf("   888888888       888888888          88\n");
  printf("   888888888       888888888          88\n");
  printf(" 88         88   88         88        88\n");
  printf(" 88         88   88         88        88\n");
  printf(" 88         88   88         88        88\n");
  printf(" 88         88   88         88        88 \n");
  printf("\n");
  printf("\n");
  printf("RULES OF GAME:\n");
  printf("1) If a player hit's on the head of another player. He/She wins the "
         "game.\n");
  printf("2) If a player is forced to hit the tail of another player.He/She "
         "loses the game.\n");
  printf("3) Any random point can be taken as a starting point.\n");
  printf("4) From then a list of valid moves will appear based on the rules of "
         "the game.\n");
  printf("5) One can move with their head. Each head point with 4 "
         "possibilities.\n");
  printf("6) A head turns into a tail with time.\n");
  printf("7) Tail disappears with time to keep number of tail point as 5 and "
         "number of head point as 4\n");
  printf("8) The outer boundary is circularly enclosed.\n");
  printf("10) Elements cannot exist inside the inner hole.\n");
  printf("11) The inner hole teleports elements diagonally.\n");
  printf("\n");
  printf("**Size of the board can be set at the beginning of execution**\n");
  printf("**Size of snake can easily be modified at the beginning of the "
         "code**\n\n\n");
  printf("**ANY STARTING MOVE IS VALID AND IT HAS TO BE ENTERED IN THE FORMAT: "
         "1B or 1c**\n\n\n");
}
int main(int argc, char const *argv[]) {
  if (argc > 1) {
    size_board = atoi(argv[1]);
    if (argc > 2) {
      size_hole = atoi(argv[2]);
    }
  }
  info();
  fill_moves();
  board = (char *)malloc(4 * size_board * size_board * sizeof(char));
  board_reset();

  char player = 'A';
  while (1) {
    board_draw();
    char target_x;
    short target_y;

    while (1) {
      possible(player);
      moves_print();

      printf("Player %c > ", player);
      if (scanf("%hu%c", &target_y, &target_x) != 2)
        continue;

      target_x = toupper(target_x) - 'A';
      if (target_x < 0 || target_x >= size_board * 2)
        continue;

      target_y -= 1;
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
    for (int i = 0; i < SIZE_SNAKE - 1; i++) {
      last_moves_x[player - 'A'][i] = last_moves_x[player - 'A'][i + 1];
      last_moves_y[player - 'A'][i] = last_moves_y[player - 'A'][i + 1];
    }
    last_moves_x[player - 'A'][SIZE_SNAKE - 1] = target_x;

    tolower(board[last_moves_y[player - 'A'][2]]);
    last_moves_y[player - 'A'][SIZE_SNAKE - 1] = target_y;

    player = (player == 'A') ? 'B' : 'A';
  }

  free(board);

  return 0;
}
