#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include <tui.h>

#define WIDTH 80
#define HEIGHT 20

#define DEAD_CELL ' '
#define ALIVE_CELL '#'

#define AT(y, x) (y * WIDTH + x)

void signal_handle(int signal)
{
    (void)signal;
    printf("Killed program\n");
    exit(EXIT_SUCCESS);
}

void fill_canvas(char *canvas)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            canvas[AT(i, j)] = DEAD_CELL;
        }
    }
}

void fill_random_canvas(char *canvas)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (rand() / (float)RAND_MAX > 0.5)
            {
                canvas[AT(i, j)] = DEAD_CELL;
            }
            else
            {
                canvas[AT(i, j)] = ALIVE_CELL;
            }
        }
    }
}

void print_canvas(char *canvas)
{
    TUIStyle s;
    s.style = STYLE_NORMAL;
    s.fg_color = FG_DEFAULT;
    s.bg_color = BG_WHITE;

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (canvas[AT(i, j)] == ALIVE_CELL)
            {
                tui_set_style(&s);
            }
            printf(" "); // printf("%c", canvas[AT(i, j)]);
            tui_reset_style();
        }
        printf("\n");
    }
}

int get_number_of_neighbors(char *canvas, int cx, int cy)
{
    int neighbors = 0;
    for (int y = cy - 1; y <= cy + 1; y++)
    {
        if (y >= 0 && y < HEIGHT)
        {
            for (int x = cx - 1; x <= cx + 1; x++)
            {
                if (x >= 0 && x < WIDTH)
                {
                    if (canvas[AT(y, x)] == ALIVE_CELL)
                    {
                        neighbors++;
                    }
                }
            }
        }
    }
    if (canvas[AT(cy, cx)] == ALIVE_CELL)
    {
        neighbors--;
    }
    return neighbors;
}

void update(char *canvas)
{
    char *back_canvas = (char *)malloc(sizeof(char) * WIDTH * HEIGHT);
    memcpy(back_canvas, canvas, sizeof(char) * WIDTH * HEIGHT);

    for (int cy = 0; cy < HEIGHT; cy++)
    {
        for (int cx = 0; cx < WIDTH; cx++)
        {
            int neighbors = get_number_of_neighbors(canvas, cx, cy);

            if (canvas[AT(cy, cx)] == ALIVE_CELL)
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    back_canvas[AT(cy, cx)] = DEAD_CELL;
                }
            }
            else if (canvas[AT(cy, cx)] == DEAD_CELL)
            {
                if (neighbors == 3)
                {
                    back_canvas[AT(cy, cx)] = ALIVE_CELL;
                }
            }
        }
    }

    memcpy(canvas, back_canvas, sizeof(char) * WIDTH * HEIGHT);
}

int main()
{
    signal(SIGINT, signal_handle);

    char canvas[HEIGHT * WIDTH];

    srand(time(NULL));
    fill_random_canvas(canvas);

    tui_enable_raw_mode();
    tui_hide_cursor();
    while (1)
    {
        tui_set_cursor_position(1, 1);
        tui_clear_screen();

        update(canvas);

        print_canvas(canvas);
        usleep(400 * 1000);
    }
    return 0;
}