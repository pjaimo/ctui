#ifndef TUI_H
#define TUI_H

#include <stdio.h>
#include <stdlib.h>

#define FG_BLACK 30
#define FG_RED 31
#define FG_GREEN 32
#define FG_YELLOW 33
#define FG_BLUE 34
#define FG_MAGENTA 35
#define FG_CYAN 36
#define FG_WHITE 37
#define FG_DEFAULT 39

#define BG_BLACK 40
#define BG_RED 41
#define BG_GREEN 42
#define BG_YELLOW 43
#define BG_BLUE 44
#define BG_MAGENTA 45
#define BG_CYAN 46
#define BG_WHITE 47
#define BG_DEFAULT 49

#define STYLE_BOLD 1
#define STYLE_DIM 2
#define STYLE_ITALIC 3
#define STYLE_UNDERLINE 4
#define STYLE_REVERSE 7
#define STYLE_HIDE 8
#define STYLE_STRIKETHROUGH 9
#define STYLE_NORMAL 22

#define set_rgb(property, r, g, b)                 \
    do                                             \
    {                                              \
        property = (int *)malloc(sizeof(int) * 3); \
        property[0] = r;                           \
        property[1] = g;                           \
        property[2] = b;                           \
    } while (0);

typedef struct
{
    int style;
    int fg_color;
    int bg_color;
    int *rgb_fg;
    int *rgb_bg;
} style_t;

void clear_screen()
{
    printf("\x1b[2J");
    fflush(stdout);
}

void set_style(style_t *s)
{
    if (s->style >= 30)
    {
        printf("Invalid style\n");
        exit(EXIT_FAILURE);
    }

    if (s->fg_color < 30 || s->fg_color >= 40)
    {
        printf("Invalid foreground color\n");
        exit(EXIT_FAILURE);
    }
    if (s->bg_color < 40 || s->bg_color >= 50)
    {
        printf("Invalid foreground color\n");
        exit(EXIT_FAILURE);
    }

    printf("\x1b[%d;%d;%dm", s->style, s->fg_color, s->bg_color);
    fflush(stdout);
}

void set_style_rgb(style_t *s)
{
    if (s->style >= 30)
    {
        printf("Invalid style\n");
        exit(EXIT_FAILURE);
    }

    if (s->rgb_fg)
    {
        printf("\x1b[%d;38;2;%d;%d;%dm", s->style, s->rgb_fg[0], s->rgb_fg[1], s->rgb_fg[2]);
        fflush(stdout);
    }
    if (s->rgb_bg)
    {
        printf("\x1b[%d;48;2;%d;%d;%dm", s->style, s->rgb_bg[0], s->rgb_bg[1], s->rgb_bg[2]);
        fflush(stdout);
    }
}

void reset_style()
{
    printf("\x1b[0m");
    fflush(stdout);
}

void hide_cursor()
{
    printf("\x1b[?25l");
    fflush(stdout);
}

void show_cursor()
{
    printf("\x1b[?25h");
    fflush(stdout);
}

void set_cursor_position(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
    fflush(stdout);
}

void move_cursor_up(int n)
{
    printf("\x1b[%dA", n);
    fflush(stdout);
}

void move_cursor_down(int n)
{
    printf("\x1b[%dB", n);
    fflush(stdout);
}

void move_cursor_forward(int n)
{
    printf("\x1b[%dC", n);
    fflush(stdout);
}

void move_cursor_back(int n)
{
    printf("\x1b[%dD", n);
    fflush(stdout);
}

void get_cursor_position()
{
    printf("\x1b[6n");
    fflush(stdout);
}

#endif // TUI_H