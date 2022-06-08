#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>

#define BORDER_CHAR '*'
#define LIVE_CELL '@'
#define DEAD_CELL ' '

typedef struct game_of_life {
  char map[25][81];
  int speed;
  int stop;
} t_life;

int read_from_file(t_life *game, char *file_name);
void ft_print_map(t_life *game);
int ft_next_step(t_life *game);
int check_cell(t_life *game, int x, int y);
void map_copy(t_life *game, char tmp_map[][81]);
int map_cmp(t_life *game, char tmp_map[][81]);
void *ft_change_speed(void *game);
int check_map_line(const char *str, int index);

int main(void) {
  t_life game;
  pthread_t pid;
  game.speed = 1000000;
  game.stop = 0;
  if (read_from_file(&game, "game_of_life_4.txt") != 0) {
    printf("n/a");
    return (1);
  }
  pthread_create(&pid, NULL, ft_change_speed, &game);
  do {
    ft_print_map(&game);
    usleep(game.speed);
  } while (ft_next_step(&game)  == 1 && game.stop == 0);
  pthread_join(pid, NULL);
  return (0);
}

int check_map_line(const char *str, int index) {
  if (index == 0 || index == 24) {
    for (int i = 0; i < 80; i++)
    if (str[i] != BORDER_CHAR)
      return (1);
  } else {
    if (str[0] != BORDER_CHAR || str[79] != BORDER_CHAR)
      return (1);
    for (int i = 1; i < 79; i++)
      if (str[i] != LIVE_CELL && str[i] != DEAD_CELL)
        return (1);
  }
  return (0);
}

int read_from_file(t_life *game, char *file_name) {
  FILE *file;
  file = fopen(file_name, "r");
  if (file == NULL)
    return (1);
  for (int i = 0; i < 25; i++) {
    if (fread(game->map[i], 1, 81, file) != 81)
      return (1);
    game->map[i][80] = '\0';
    if (check_map_line(game->map[i], i) == 1)
      return (1);
  }
  fclose(file);
  return (0);
}

void ft_print_map(t_life *game) {
  printf("wait_time : %d ms\n", game->speed / 1000);
  for (int i = 0; i < 25; i++)
    printf("%s\n", game->map[i]);
  return;
}

int ft_next_step(t_life *game) {
  char tmp_map[25][81];
  int neighbors;
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 80; j++) {
      neighbors = check_cell(game, i, j);
      if (game->map[i][j] == LIVE_CELL && (neighbors < 2 || neighbors > 3))
        tmp_map[i][j] = DEAD_CELL;
      else if (game->map[i][j] == DEAD_CELL && neighbors == 3)
        tmp_map[i][j] = LIVE_CELL;
      else
        tmp_map[i][j] = game->map[i][j];
    }
  }
  if (map_cmp(game, tmp_map) == 1)
    return (0);
  map_copy(game, tmp_map);
  return (1);
}

void ft_get_directions(int *up, int *down, int *right, int *left) {
  if (*up == 0)
    *up = 23;
  if (*down == 24)
    *down = 1;
  if (*right == 79)
    *right = 1;
  if (*left == 0)
    *left = 78;
}

int check_cell(t_life *game, int i, int j) {
  int n;
  int up, down, right, left;
  if (i > 23 || i < 1 || j < 1 || j > 78)
    return (0);
  up = i - 1;
  down = i + 1;
  right = j + 1;
  left = j - 1;
  ft_get_directions(&up, &down, &right, &left);
  n = 0;
  if (game->map[up][j] == LIVE_CELL)
    n++;
  if (game->map[down][j] == LIVE_CELL)
    n++;
  if (game->map[i][left] == LIVE_CELL)
    n++;
  if (game->map[i][right] == LIVE_CELL)
    n++;
  if (game->map[down][left] == LIVE_CELL)
    n++;
  if (game->map[down][right] == LIVE_CELL)
    n++;
  if (game->map[up][left] == LIVE_CELL)
    n++;
  if (game->map[up][right] == LIVE_CELL)
    n++;
  return (n);
}

void map_copy(t_life *game, char tmp_map[][81]) {
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 80; j++) {
      game->map[i][j] = tmp_map[i][j];
    }
  }
  return;  // ??
}

int map_cmp(t_life *game, char tmp_map[][81]) {
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 80; j++) {
      if (game->map[i][j] != tmp_map[i][j])
       return (0);
    }
  }
  return (1);
}

void *ft_change_speed(void *arg) {
  int c;
  t_life *game;
  struct termios oldterminal, newterminal;
  tcgetattr(STDIN_FILENO, &oldterminal);
  newterminal = oldterminal;
  newterminal.c_lflag &= ~(ICANON | ECHO);
  newterminal.c_cc[VMIN] = 1;
  newterminal.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &newterminal);
  game = (t_life *)arg;
  c = getc(stdin);
  while (c != 'q' && c != 'Q') {
    if ((c == 'a' || c == 'A') && game->speed <= 3000000)
      game->speed += 100000;
    else if ((c == 'z' || c == 'Z') && game->speed >= 200000)
      game->speed -= 100000;
    c = getc(stdin);
  }
  game->stop = 1;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldterminal);
  return (NULL);
}
