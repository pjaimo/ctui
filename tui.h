#ifndef TUI_H
#define TUI_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifdef __linux__
#include <termios.h>
#endif

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

#define SPIN_DURATION 200 * 1000

typedef struct
{
    int style;
    int fg_color;
    int bg_color;
    int *rgb_fg;
    int *rgb_bg;
} TUIStyle;

#ifdef __linux__
struct termios original_term;

void tui_disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
}

void tui_enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &original_term);

    struct termios raw = original_term;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    atexit(tui_disable_raw_mode);
}

void tui_get_cursor_position(int *x, int *y)
{
    printf("\x1b[6n");
    fflush(stdout);

    char buf[20] = {0};
    int i = 0;
    char c;
    while (c != 'R')
    {
        read(STDIN_FILENO, &c, 1);
        buf[i] = c;
        i++;
    }

    char xs[6] = {0};
    char ys[6] = {0};
    for (int j = 2, k = 0, h = 0, next = 0; j < i - 1; j++)
    {
        if (buf[j] == ';')
        {
            next = 1;
            continue;
        }
        if (next == 0)
        {
            ys[k] = buf[j];
            k++;
        }
        else
        {
            xs[h] = buf[j];
            h++;
        }
    }
    *x = atoi(xs);
    *y = atoi(ys);
}

#endif
void tui_clear_screen()
{
    printf("\x1b[2J");
    fflush(stdout);
}

void tui_set_style(TUIStyle *s)
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

void tui_set_style_rgb(TUIStyle *s)
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

void tui_reset_style()
{
    printf("\x1b[0m");
    fflush(stdout);
}

void tui_show_cursor()
{
    printf("\x1b[?25h");
    fflush(stdout);
}

void tui_hide_cursor()
{
    printf("\x1b[?25l");
    fflush(stdout);
    atexit(tui_show_cursor);
}

void tui_set_cursor_position(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
    fflush(stdout);
}

void tui_move_cursor_up(int n)
{
    printf("\x1b[%dA", n);
    fflush(stdout);
}

void tui_move_cursor_down(int n)
{
    printf("\x1b[%dB", n);
    fflush(stdout);
}

void tui_move_cursor_forward(int n)
{
    printf("\x1b[%dC", n);
    fflush(stdout);
}

void tui_move_cursor_back(int n)
{
    printf("\x1b[%dD", n);
    fflush(stdout);
}

/* HIGH LEVEL FUNCTIONS */
typedef struct
{
    int x;
    int y;
} Vector2;

void tui_create_canvas(char fill_char, int width, int height)
{
    tui_clear_screen();
    tui_set_cursor_position(1, 1);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c", fill_char);
        }
        printf("\n");
    }
}

void print_ascii_unicode(char *ascii, char *unicode)
{
#ifdef TUI_UNICODE
    (void)ascii;
    printf("%s", unicode);
#else
    (void)unicode;
    printf("%s", ascii);
#endif
}

typedef struct
{
    Vector2 pos;
    int width;
    int height;
} TUIRect;

void tui_add_rect(const TUIRect *rect, const char fill_char)
{
    for (int j = 0; j < rect->height; j++)
    {
        tui_set_cursor_position(rect->pos.x, rect->pos.y + j);
        if (j == 0)
        {
            print_ascii_unicode("+", "┏");
            for (int i = 0; i < rect->width - 2; i++)
            {
                print_ascii_unicode("-", "━");
            }
            print_ascii_unicode("+", "┓");
        }
        else if (j == rect->height - 1)
        {
            print_ascii_unicode("+", "┗");
            for (int i = 0; i < rect->width - 2; i++)
            {
                print_ascii_unicode("-", "━");
            }
            print_ascii_unicode("+", "┛");
        }
        else
        {
            print_ascii_unicode("|", "┃");

            for (int i = 0; i < rect->width - 2; i++)
            {
                printf("%c", fill_char);
            }
            print_ascii_unicode("|", "┃");
        }
    }
    tui_set_cursor_position(9999, 9999);
}

typedef struct
{
    Vector2 pos;
    int radius;
} TUICircle;

void tui_add_circle(const TUICircle *circle, const char fill_char)
{
    for (int i = -circle->radius; i < circle->radius; i++)
    {
        for (int j = -circle->radius; j < circle->radius; j++)
        {
            if (i * i + j * j < circle->radius * circle->radius)
            {
                tui_set_cursor_position(circle->pos.x + j + 1, circle->pos.y + i + 1);
                putchar(fill_char);
            }
        }
    }
    tui_set_cursor_position(9999, 9999);
}

void tui_textbox(const char *text, Vector2 pos)
{
    TUIRect r = {.pos = pos, .height = 3, .width = strlen(text) + 4}; // With one cell of horizontal spacing
    tui_add_rect(&r, ' ');
    tui_set_cursor_position(pos.x + 2, pos.y + 1);
    printf("%s", text);
    tui_set_cursor_position(9999, 9999);
}

enum Spinner
{
    LINE,
    DOTS
};

void tui_spinner(enum Spinner spinner)
{
    switch (spinner)
    {
    case LINE:
    {
        char *lineframes[] = {"-", "\\", "|", "/"};
        for (int i = 0; i < 4; i++)
        {
            printf("%s", lineframes[i]);
            tui_move_cursor_back(1);
            usleep(SPIN_DURATION);
        }
        break;
    }
    case DOTS:
    {
        char *dotframes[] = {"⠋",
                             "⠙",
                             "⠹",
                             "⠸",
                             "⠼",
                             "⠴",
                             "⠦",
                             "⠧",
                             "⠇",
                             "⠏"};

        for (int i = 0; i < 10; i++)
        {
            printf("%s", dotframes[i]);
            tui_move_cursor_back(1);
            usleep(SPIN_DURATION);
        }
        break;
    }
    }
}

#endif // TUI_H