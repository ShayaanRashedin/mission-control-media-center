/* Game rules, updated every 50 ms. */
#include "games.h"

/* Rescue maps */
static const char *const maze[2][9] = {{"################", "#..............#", "#..###..###....#",
                                        "#....#....#....#", "#....#.........#", "#.......###....#",
                                        "#..###.........#", "#..............#", "################"},
                                       {"################", "#..............#", "#.####..####...#",
                                        "#..............#", "#..##..##..##..#", "#......#.......#",
                                        "#..............#", "#..............#",
                                        "################"}};
int rescue_wall(unsigned level, int x, int y) {
    if (x < 0 || x >= 16 || y < 0 || y >= 9)
        return 1;
    return maze[level == 2 ? 1 : 0][y][x] == '#';
}

/* Supplies and battery packs */
unsigned rescue_supply(unsigned level, int x, int y) {
    if (level == 2) {
        if (x == 12 && y == 1)
            return 1;
        if (x == 3 && y == 3)
            return 2;
        if (x == 13 && y == 5)
            return 4;
        if (x == 4 && y == 7)
            return 8;
    } else {
        if (x == 6 && y == 1)
            return 1;
        if (x == 13 && y == 3)
            return 2;
        if (x == 2 && y == 6)
            return 4;
        if (x == 10 && y == 7)
            return 8;
    }
    return 0;
}
unsigned rescue_battery(unsigned level, int x, int y) {
    if (level == 2) {
        if (x == 6 && y == 5)
            return 1;
        if (x == 12 && y == 7)
            return 2;
    } else {
        if (x == 1 && y == 4)
            return 1;
        if (x == 12 && y == 6)
            return 2;
    }
    return 0;
}

/* Moving red hazards */
static void hazard_at(unsigned level, unsigned frame, unsigned n, int *x, int *y) {
    unsigned span = n ? 7 : 3;
    unsigned step = frame / (level == 2 ? 6 : 8);
    unsigned phase = (step + (n ? 5 : 0)) % (span * 2);
    if (phase > span)
        phase = span * 2 - phase;
    *x = 6 + (int)phase;
    *y = n ? 6 : 3;
}
void rescue_hazard(const Game *g, unsigned n, int *x, int *y) {
    hazard_at(g->level, g->frame, n, x, y);
}

/* Landing pad */
unsigned lander_pad_x(const Game *g) { return g->level == 2 ? 95 : 220; }
unsigned lander_pad_width(const Game *g) { return g->level == 2 ? 50 : 65; }

/* Level setup */
static void level_start(Game *g) {
    g->result = GAME_RUNNING;
    g->frame = g->collected = g->supplies = g->batteries = 0;
    g->vx = g->vy = g->wind = 0;
    g->move_wait = g->flash = g->hit_wait = 0;
    g->lives = 3;
    g->fuel = g->level == 2 ? 340 : 400;
    g->battery = g->level == 2 ? 1000 : 1200;
    g->x = g->kind == GAME_LANDER ? (g->level == 2 ? 2700 : 400) : 1;
    g->y = g->kind == GAME_LANDER ? 550 : 1;
}
void game_start(Game *g, unsigned kind) {
    g->kind = kind;
    g->level = 1;
    g->score = 0;
    level_start(g);
}
void game_next(Game *g) {
    if (g->result != GAME_LEVEL_CLEAR || g->level >= GAME_LEVELS)
        return;
    g->level++;
    level_start(g);
}

/* Finish a level */
static void level_won(Game *g, unsigned points) {
    g->score += points;
    g->result = g->level < GAME_LEVELS ? GAME_LEVEL_CLEAR : GAME_WON;
}

/* Landing physics */
static void lander_step(Game *g, unsigned held) {
    int thrust = (held & KEY_UP) && g->fuel;
    int pad = (int)lander_pad_x(g) * 10;
    int width = (int)lander_pad_width(g) * 10;
    int speed = g->level == 2 ? 10 : 12;
    int drift = g->level == 2 ? 6 : 8;
    g->wind = g->level == 2 ? ((g->frame / 80) & 1 ? -1 : 1) : 0;
    if (g->frame % 10 == 0)
        g->vx += g->wind;
    g->vy++;
    if (thrust) {
        g->vy -= 3;
        g->fuel--;
    }
    if ((held & KEY_LEFT) && g->fuel) {
        g->vx--;
        g->fuel--;
    }
    if ((held & KEY_RIGHT) && g->fuel) {
        g->vx++;
        g->fuel--;
    }
    if (g->vx < -20)
        g->vx = -20;
    if (g->vx > 20)
        g->vx = 20;
    if (g->vy < -20)
        g->vy = -20;
    if (g->vy > 35)
        g->vy = 35;
    g->x += g->vx;
    g->y += g->vy;
    if (g->x < 100) {
        g->x = 100;
        g->vx = 0;
    }
    if (g->x > 3090) {
        g->x = 3090;
        g->vx = 0;
    }
    if (g->y < 450) {
        g->y = 450;
        g->vy = 0;
    }
    if (g->y >= 1800) {
        g->y = 1800;
        if (g->x >= pad + 80 && g->x <= pad + width - 80 && g->vy <= speed && g->vx >= -drift &&
            g->vx <= drift)
            level_won(g, 200 + g->fuel * 2 + (unsigned)(speed - g->vy) * 10);
        else
            g->result = GAME_LOST;
    }
}

/* Robot movement */
static void rescue_step(Game *g, unsigned held) {
    int x = g->x, y = g->y, old_x = x, old_y = y;
    int hx, hy, prev_x, prev_y;
    unsigned bit, n, cap = g->level == 2 ? 1000 : 1200;
    if (g->battery)
        g->battery--;
    if (g->flash)
        g->flash--;
    if (g->hit_wait)
        g->hit_wait--;
    if (g->move_wait)
        g->move_wait--;
    if (!g->move_wait) {
        if (held & KEY_UP)
            y--;
        else if (held & KEY_DOWN)
            y++;
        else if (held & KEY_LEFT)
            x--;
        else if (held & KEY_RIGHT)
            x++;
        if (x != g->x || y != g->y) {
            g->move_wait = 3;
            if (rescue_wall(g->level, x, y)) {
                g->battery = g->battery > 40 ? g->battery - 40 : 0;
                g->flash = 3;
            } else {
                g->x = x;
                g->y = y;
            }
        }
    }
    bit = rescue_supply(g->level, g->x, g->y);
    if (bit && !(g->supplies & bit)) {
        g->supplies |= bit;
        g->collected++;
        g->score += 100;
    }
    bit = rescue_battery(g->level, g->x, g->y);
    if (bit && !(g->batteries & bit)) {
        g->batteries |= bit;
        g->battery += 240;
        if (g->battery > cap)
            g->battery = cap;
        g->score += 25;
    }
    /* One second of protection after a hit. */
    for (n = 0; n < 2 && !g->hit_wait; n++) {
        rescue_hazard(g, n, &hx, &hy);
        hazard_at(g->level, g->frame - 1, n, &prev_x, &prev_y);
        if ((g->x == hx && g->y == hy) ||
            (g->x == prev_x && g->y == prev_y && old_x == hx && old_y == hy)) {
            if (g->lives)
                g->lives--;
            g->battery = g->battery > 100 ? g->battery - 100 : 0;
            g->hit_wait = g->flash = 20;
        }
    }
    if (!g->lives || !g->battery)
        g->result = GAME_LOST;
    else if (g->x == 14 && g->y == 7 && g->collected == 4)
        level_won(g, (g->battery / 20) * 5 + g->lives * 50);
}

/* One game update */
void game_step(Game *g, unsigned held) {
    if (g->result != GAME_RUNNING)
        return;
    g->frame++;
    if (g->kind == GAME_LANDER)
        lander_step(g, held);
    else
        rescue_step(g, held);
}
