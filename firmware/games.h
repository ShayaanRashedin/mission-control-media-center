/* Game state and controls. */
#ifndef GAMES_H
#define GAMES_H
#define KEY_UP (1U << 1)
#define KEY_DOWN (1U << 2)
#define KEY_LEFT (1U << 3)
#define KEY_RIGHT (1U << 4)
#define KEY_SELECT (1U << 5)
#define GAME_LANDER 0
#define GAME_RESCUE 1
#define GAME_RUNNING 0
#define GAME_WON 1
#define GAME_LOST 2
#define GAME_LEVEL_CLEAR 3
#define GAME_LEVELS 2
typedef struct {
    unsigned kind, result, frame, fuel, battery, collected, supplies;
    int x, y, vx, vy;
    unsigned move_wait, flash;
    unsigned level, score, lives, batteries, hit_wait;
    int wind;
} Game;
void game_start(Game *g, unsigned kind);
void game_next(Game *g);
void game_step(Game *g, unsigned held);
unsigned lander_pad_x(const Game *g);
unsigned lander_pad_width(const Game *g);
int rescue_wall(unsigned level, int x, int y);
unsigned rescue_supply(unsigned level, int x, int y);
unsigned rescue_battery(unsigned level, int x, int y);
void rescue_hazard(const Game *g, unsigned n, int *x, int *y);
#endif
