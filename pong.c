#include <stdio.h>

typedef struct s_vect {
  float x;
  float y;
} t_vect;

typedef struct s_pong {
  char map[25][81];
  t_vect player_1;
  t_vect player_2;
  t_vect ball;
  t_vect direction;
} t_pong;

typedef struct s_game {
  t_pong pong;
  int score_1;
  int score_2;
  char border;
  char racket_char;
  char ball_char;
} t_game;

void ft_init(t_game *game) {
  game->score_1 = 0;
  game->score_2 = 0;
  game->border = '*';
  game->racket_char = '|';
  game->ball_char = 'O';
  game->pong.player_1.x = 12;
  game->pong.player_1.y = 1;
  game->pong.player_2.x = 12;
  game->pong.player_2.y = 78;
  game->pong.ball.x = 12;
  game->pong.ball.y = 2;
  game->pong.direction.x = 0;
  game->pong.direction.y = 1;
}

void ft_print_map(t_game *game) {
  int i, j;

  printf("\n       player 1 :  %d              player 2 : %d \n", game->score_1, game->score_2);
  i = 0;
  while (i < 25) {
    j = 0;
    while (j < 80) {
      if (i == 0 || i == 24) {
        game->pong.map[i][j] = game->border;
      } else if (j == 0 || j == 79) {
        game->pong.map[i][j] = game->border;
      } else {
        game->pong.map[i][j] = ' ';
      }
      j++;
    }
    game->pong.map[i][j] = 0;
    i++;
  }
  i = game->pong.player_1.x;
  j = game->pong.player_2.x;
  game->pong.map[i - 1][1] = game->racket_char;
  game->pong.map[i][1] = game->racket_char;
  game->pong.map[i + 1][1] = game->racket_char;
  game->pong.map[j - 1][78] = game->racket_char;
  game->pong.map[j][78] = game->racket_char;
  game->pong.map[j + 1][78] = game->racket_char;
  game->pong.map[(int)game->pong.ball.x][(int)game->pong.ball.y] = game->ball_char;
  i = 0;
  while (i < 25) {
    printf("%s\n", game->pong.map[i++]);
  }
  printf("ball (%f, %f)     direction (%f, %f)\n", game->pong.ball.x, game->pong.ball.y, \
  game->pong.direction.x, game->pong.direction.y);
}

char *ft_strchr(char *str, int c) {
  if (str == NULL)
    return (NULL);
  while (*str) {
    if (*str == c)
      return (str);
    str++;
  }
  return (NULL);
}

/********* move ball *********/

void ft_intersect_goal(t_game *game) {
  if (game->pong.ball.y == 0) {
    game->score_2++;
    game->pong.ball.x = 12;
    game->pong.ball.y = 2;
    game->pong.direction.x = 0;
    game->pong.direction.y = 1;
  } else if (game->pong.ball.y == 79) {
    game->score_1++;
    game->pong.ball.x = 12;
    game->pong.ball.y = 77;
    game->pong.direction.x = 0;
    game->pong.direction.y = -1;
  }
}

void ft_intersect_wall(t_game *game) {
  if (game->pong.ball.x == 0 || game->pong.ball.x == 24) {
    game->pong.ball.x -= game->pong.direction.x;
    game->pong.direction.x *= -1;
    game->pong.ball.x += game->pong.direction.x;
  }
}

void ft_intersect_racket_left(t_game *game) {
  if (game->pong.ball.y == 1) {
    game->pong.ball.x -= game->pong.direction.x;
    game->pong.ball.y -= game->pong.direction.y;
    if (game->pong.ball.x == game->pong.player_1.x - 1) {
      if (game->pong.direction.x == 0) {
        game->pong.direction.x = -1;
        game->pong.direction.y *= -1;
      } else if (game->pong.direction.x > 0) {
        game->pong.direction.x *= 0.5;
        game->pong.direction.y *= -1;
      } else {
        game->pong.direction.x *= 1;
        game->pong.direction.y *= -0.5;
      }
    } else if (game->pong.ball.x == game->pong.player_1.x) {
      game->pong.direction.y *= -1;
    } else if (game->pong.ball.x == game->pong.player_1.x + 1) {
      if (game->pong.direction.x == 0) {
        game->pong.direction.x = 1;
        game->pong.direction.y *= -1;
      } else if (game->pong.direction.x > 0) {
        game->pong.direction.x *= 1;
        game->pong.direction.y *= -0.5;
      } else {
        game->pong.direction.x *= -1;
        game->pong.direction.y *= 0.5;
      }
    }
    game->pong.ball.x += game->pong.direction.x;
    game->pong.ball.y += game->pong.direction.y;
  }
}

void ft_intersect_racket_right(t_game *game) {
  if (game->pong.ball.y == 78) {
    game->pong.ball.x -= game->pong.direction.x;
    game->pong.ball.y -= game->pong.direction.y;
    if (game->pong.ball.x == game->pong.player_2.x - 1) {
      if (game->pong.direction.x == 0) {
        game->pong.direction.x = -1;
        game->pong.direction.y *= -1;
      } else if (game->pong.direction.x > 0) {
        game->pong.direction.x *= 0.5;
        game->pong.direction.y *= -1;
      } else {
        game->pong.direction.x *= 1;
        game->pong.direction.y *= -0.5;
      }
    } else if (game->pong.ball.x == game->pong.player_2.x) {
      game->pong.direction.y *= -1;
    } else if (game->pong.ball.x == game->pong.player_2.x + 1) {
      if (game->pong.direction.x == 0) {
        game->pong.direction.x = 1;
        game->pong.direction.y *= -1;
      } else if (game->pong.direction.x > 0) {
        game->pong.direction.x *= 1;
        game->pong.direction.y *= -0.5;
      } else {
        game->pong.direction.x *= -1;
        game->pong.direction.y *= 0.5;
      }
    }
    game->pong.ball.x += game->pong.direction.x;
    game->pong.ball.y += game->pong.direction.y;
  }
}

void ft_move_ball(t_game *game) {
  game->pong.ball.x += game->pong.direction.x;
  game->pong.ball.y += game->pong.direction.y;
  ft_intersect_goal(game);
  ft_intersect_racket_left(game);
  ft_intersect_racket_right(game);
  ft_intersect_wall(game);
}

/**********end move ball *******/

int  ft_game_end(t_game *game, int max_points) {
  if (game->score_2 >= max_points || game->score_1 >= max_points)
    return (1);
  return (0);
}

void  ft_print_end_msg(t_game *game, int max_points) {
  if (game->score_1 >= max_points)
    printf("Congratulations Player 1\n");
  else
    printf("Congratulations Player 2\n");
}

void ft_next(t_game *game) {
  int c;

  c = getc(stdin);
  if (ft_strchr("azkm ", c)) {
    if (c == 'a') {
      if (game->pong.player_1.x > 2)
        game->pong.player_1.x--;
    } else if (c == 'z') {
      if (game->pong.player_1.x < 22)
        game->pong.player_1.x++;
    } else if (c == 'k') {
        if (game->pong.player_2.x > 2)
          game->pong.player_2.x--;
    } else if (c == 'm') {
        if (game->pong.player_2.x < 22)
          game->pong.player_2.x++;
    }
    ft_move_ball(game);
    ft_print_map(game);
  }
}

int main() {
    t_game game;

  ft_init(&game);
  ft_print_map(&game);
  while (1) {
    ft_next(&game);
    if (ft_game_end(&game, 21))
      break;
  }
  ft_print_end_msg(&game, 21);
  return 0;
}
