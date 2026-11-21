/* Screens, controls and game drawing. */
#include "media.h"
#include "games.h"
#include "GLCD.h"
#include "pictures.h"
static unsigned mode, item, photo, last_tick, last_audio;
static unsigned old_result, old_flash, old_frame;
static int old_x, old_y;
static Game game;
static unsigned best_score[2];
static int old_hx[2], old_hy[2];
static const char *const names[] = {"Photo Gallery", "MP3 Player", "Lunar Lander", "Robot Rescue"};

/* Short text helpers */
static char *word(char *out, const char *s) {
    while (*s)
        *out++ = *s++;
    *out = 0;
    return out;
}
static char *number(char *out, int value) {
    char digits[12];
    unsigned n = 0, v;
    if (value < 0) {
        *out++ = '-';
        v = (unsigned)(-value);
    } else
        v = (unsigned)value;
    do {
        digits[n++] = (char)('0' + v % 10);
        v /= 10;
    } while (v);
    while (n)
        *out++ = digits[--n];
    *out = 0;
    return out;
}
/* Screen text */
static void line(unsigned row, const char *s) {
    GLCD_ClearLn(row, 1);
    GLCD_DisplayString(row, 0, 1, (unsigned char *)s);
}
static void small(unsigned row, const char *s) {
    GLCD_ClearLn(row, 0);
    GLCD_DisplayString(row, 0, 0, (unsigned char *)s);
}
static void clear(void) {
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Cyan);
    GLCD_Clear(Black);
}

/* Menu and gallery */
static void draw(void) {
    unsigned i;
    clear();
    line(0, "MISSION CONTROL");
    GLCD_SetTextColor(White);
    if (mode == MENU) {
        for (i = 0; i < 4; i++) {
            GLCD_SetBackColor(i == item ? Blue : Black);
            line(i + 2, names[i]);
        }
        GLCD_SetBackColor(Black);
        line(8, "Up/Down: choose");
        line(9, "Press: open");
    } else if (mode == GALLERY) {
        line(1, picture_names[photo]);
        GLCD_Bitmap(80, 60, 160, 120, (unsigned char *)pictures[photo]);
        line(8, "Left/Right: browse");
        line(9, "Press: back");
    } else if (mode == AUDIO) {
        line(2, "USB AUDIO");
        line(6, "Knob: volume");
        small(24, "Left: retry USB  Right: speaker test");
        line(9, "Press: back");
        last_audio = ~0U;
    }
}

/* Spacecraft */
static void ship(int x, int y, unsigned flame, unsigned short colour) {
    GLCD_FillRect((unsigned)(x - 5), (unsigned)(y - 6), 10, 10, colour);
    GLCD_FillRect((unsigned)(x - 8), (unsigned)(y + 4), 3, 6, colour);
    GLCD_FillRect((unsigned)(x + 5), (unsigned)(y + 4), 3, 6, colour);
    if (flame)
        GLCD_FillRect((unsigned)(x - 2), (unsigned)(y + 5), 4, 5, Yellow);
}
static void robot(int x, int y) {
    unsigned px = 16 + 18 * x, py = 44 + 18 * y;
    GLCD_FillRect(px + 3, py + 3, 12, 12,
                  game.hit_wait ? ((game.frame & 2U) ? White : Cyan) : (game.flash ? Red : Cyan));
    GLCD_FillRect(px + 5, py + 6, 2, 2, Black);
    GLCD_FillRect(px + 11, py + 6, 2, 2, Black);
    GLCD_FillRect(px + 4 + ((game.frame / 4) & 1U), py + 15, 3, 2, White);
    GLCD_FillRect(px + 11 - ((game.frame / 4) & 1U), py + 15, 3, 2, White);
}

/* Rescue map */
static void cell(int x, int y) {
    unsigned px = 16 + 18 * x, py = 44 + 18 * y;
    unsigned supply = rescue_supply(game.level, x, y);
    unsigned pack = rescue_battery(game.level, x, y);
    GLCD_FillRect(px, py, 18, 18, Black);
    if (rescue_wall(game.level, x, y))
        GLCD_FillRect(px + 1, py + 1, 16, 16, DarkGrey);
    else if (x == 14 && y == 7) {
        GLCD_FillRect(px + 1, py + 1, 16, 16, Green);
        GLCD_FillRect(px + 5, py + 5, 8, 8, Black);
    } else if (supply && !(game.supplies & supply))
        GLCD_FillRect(px + 6, py + 6, 6, 6, Yellow);
    else if (pack && !(game.batteries & pack)) {
        GLCD_FillRect(px + 5, py + 4, 8, 11, Blue);
        GLCD_FillRect(px + 8, py + 6, 2, 7, White);
        GLCD_FillRect(px + 6, py + 8, 6, 2, White);
    }
}

/* Red patrol robots */
static void hazard(int x, int y) {
    unsigned px = 16 + 18 * x, py = 44 + 18 * y;
    GLCD_FillRect(px + 3, py + 3, 12, 12, Red);
    GLCD_FillRect(px + 5, py + 6, 8, 2, White);
}

/* Game screen */
static void game_screen(void) {
    int x, y;
    unsigned n;
    clear();
    small(0, game.kind == GAME_LANDER ? "LUNAR LANDER" : "ROBOT RESCUE");
    GLCD_SetTextColor(White);
    if (game.kind == GAME_LANDER) {
        GLCD_FillRect(0, 190, 320, 2, DarkGrey);
        GLCD_FillRect(lander_pad_x(&game), 190, lander_pad_width(&game), 4, Green);
        small(26, "Up: thrust  Left/Right: steer  Press: exit");
        ship(game.x / 10, game.y / 10, 0, White);
    } else {
        for (y = 0; y < 9; y++)
            for (x = 0; x < 16; x++)
                cell(x, y);
        for (n = 0; n < 2; n++) {
            rescue_hazard(&game, n, &old_hx[n], &old_hy[n]);
            hazard(old_hx[n], old_hy[n]);
        }
        small(26, "Yellow: supplies Blue: +12s Red: avoid");
        small(28, "Get 4 supplies then dock  Press: exit");
        robot(game.x, game.y);
    }
    old_x = game.x;
    old_y = game.y;
    old_result = GAME_RUNNING;
    old_frame = ~0U;
    old_flash = 0;
}

/* Level and score */
static void score_text(void) {
    char text[54], *end;
    end = number(word(text, "Level:"), (int)game.level);
    end = word(end, "/2 Score:");
    number(end, (int)game.score);
    small(1, text);
}

/* Moving objects and scores */
static void game_draw(unsigned held) {
    char text[54], *end;
    unsigned n, packs;
    int hx[2], hy[2];
    if (game.result != GAME_RUNNING) {
        if (old_result == GAME_RUNNING) {
            if (game.score > best_score[game.kind])
                best_score[game.kind] = game.score;
            clear();
            GLCD_SetTextColor(game.result == GAME_LOST ? Red : Green);
            line(2, game.result == GAME_LEVEL_CLEAR ? "LEVEL COMPLETE"
                    : game.result == GAME_WON       ? "MISSION COMPLETE"
                                                    : "MISSION FAILED");
            GLCD_SetTextColor(White);
            if (game.kind == GAME_LANDER)
                line(4, game.result == GAME_LOST ? "Hard landing" : "Safe landing!");
            else
                line(4, game.result != GAME_LOST ? "Robot rescued!"
                        : !game.lives            ? "Robot damaged"
                                                 : "Battery empty");
            number(word(text, "Score: "), (int)game.score);
            line(5, text);
            number(word(text, "Best: "), (int)best_score[game.kind]);
            line(6, text);
            line(8, game.result == GAME_LEVEL_CLEAR ? "Right: next level" : "Right: restart");
            line(9, "Press: menu");
        }
        old_result = game.result;
        return;
    }
    if (game.kind == GAME_LANDER) {
        GLCD_FillRect((unsigned)(old_x / 10 - 8), (unsigned)(old_y / 10 - 6), 16, 16, Black);
        ship(game.x / 10, game.y / 10, (held & KEY_UP) && game.fuel, White);
        if (old_frame / 4 != game.frame / 4) {
            score_text();
            end = number(word(text, "Fuel:"), (int)game.fuel);
            end = number(word(end, " Vx:"), game.vx * 2);
            word(number(word(end, " Vy:"), game.vy * 2), " px/s");
            small(2, text);
            GLCD_FillRect(8, 34, 304, 4, DarkGrey);
            GLCD_FillRect(8, 34, game.fuel * 304 / (game.level == 2 ? 340 : 400), 4, Green);
            word(text,
                 game.level == 2 ? "Safe |Vx|<=12 Vy<=20  Wind:" : "Safe |Vx|<=16 Vy<=24  Wind:");
            end = text;
            while (*end)
                end++;
            word(end, game.wind < 0 ? "left" : game.wind > 0 ? "right" : "none");
            small(28, text);
        }
    } else {
        for (n = 0; n < 2; n++)
            rescue_hazard(&game, n, &hx[n], &hy[n]);
        if (old_x != game.x || old_y != game.y || old_flash != game.flash ||
            old_frame / 4 != game.frame / 4 || hx[0] != old_hx[0] || hx[1] != old_hx[1]) {
            cell(old_x, old_y);
            for (n = 0; n < 2; n++)
                cell(old_hx[n], old_hy[n]);
            for (n = 0; n < 2; n++) {
                hazard(hx[n], hy[n]);
                old_hx[n] = hx[n];
                old_hy[n] = hy[n];
            }
            robot(game.x, game.y);
        }
        if (old_frame / 4 != game.frame / 4) {
            score_text();
            end = number(word(text, "Battery:"), (int)((game.battery + 19) / 20));
            word(number(word(end, "s Supplies:"), (int)game.collected), "/4");
            small(2, text);
            packs = (game.batteries & 1) + ((game.batteries >> 1) & 1);
            end = number(word(text, "Health:"), (int)game.lives);
            word(number(word(end, " Packs:"), (int)packs), "/2  Walls cost 2s");
            small(3, text);
        }
    }
    old_x = game.x;
    old_y = game.y;
    old_flash = game.flash;
    old_frame = game.frame;
}

/* Menu setup */
void media_init(void) {
    mode = MENU;
    item = photo = last_tick = 0;
    draw();
}

/* Button presses */
void media_key(unsigned key) {
    if (key == NONE)
        return;
    if (mode == MENU) {
        if (key == UP)
            item = (item + 3) % 4;
        else if (key == DOWN)
            item = (item + 1) % 4;
        else if (key == SELECT) {
            mode = item + 1;
            if (mode == LANDER || mode == RESCUE) {
                game_start(&game, mode == LANDER ? GAME_LANDER : GAME_RESCUE);
                game_screen();
                game_draw(0);
                return;
            }
            draw();
            if (mode == AUDIO)
                audio_start();
            return;
        } else
            return;
    } else if (key == SELECT) {
        if (mode == AUDIO)
            audio_stop();
        mode = MENU;
    } else if (mode == GALLERY && (key == LEFT || key == RIGHT)) {
        photo = (photo + (key == RIGHT ? 1 : 2)) % 3;
    } else if ((mode == LANDER || mode == RESCUE) && game.result && key == RIGHT) {
        if (game.result == GAME_LEVEL_CLEAR)
            game_next(&game);
        else
            game_start(&game, game.kind);
        game_screen();
        game_draw(0);
        return;
    } else if (mode == AUDIO && key == LEFT) {
        audio_stop();
        audio_start();
        return;
    } else if (mode == AUDIO && key == RIGHT) {
        audio_test();
        return;
    } else
        return;
    draw();
}

/* Timed updates */
void media_update(unsigned now, unsigned held) {
    unsigned status;
    char text[54];
    if (mode == AUDIO) {
        audio_poll(now);
        status = audio_status() * 1024 + audio_volume() + audio_error() * 65536;
        if (now - last_tick >= 250) {
            last_tick = now;
            if (status != last_audio) {
                last_audio = status;
                if (audio_error() && audio_status() != 4)
                    number(word(text, "USB error: "), (int)audio_error());
                else {
                    switch (audio_status()) {
                    case 1:
                        word(text, "Waiting for USB");
                        break;
                    case 2:
                        word(text, "USB ready");
                        break;
                    case 3:
                        word(text, "Streaming audio");
                        break;
                    case 4:
                        word(text, "Speaker test");
                        break;
                    case 5:
                        word(text, "Reconnecting USB");
                        break;
                    default:
                        word(text, "Audio stopped");
                        break;
                    }
                }
                line(4, text);
                word(number(word(text, "Volume: "), (int)audio_volume()), "%");
                line(7, text);
            }
        }
    } else if (mode == LANDER || mode == RESCUE) {
        if (now - last_tick >= 50) {
            /* Drop excess delay instead of running many frames at once. */
            last_tick = now;
            game_step(&game, held);
            game_draw(held);
        }
    } else
        last_tick = now;
}
