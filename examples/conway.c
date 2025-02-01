#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include <tui.h>

#define WIDTH 40
#define HEIGHT 20

#define DEAD_CELL ' '
#define ALIVE_CELL '#'

#define AT(y, x) ((y) * WIDTH + (x))

void signal_handle(int signal)
{
    (void)signal;

    tui_clear_screen();
    tui_set_cursor_position(1, 1);
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
            printf("  "); // printf("%c", canvas[AT(i, j)]);
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
    char back_canvas[WIDTH * HEIGHT];
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
    srand(time(NULL));

    char canvas[HEIGHT * WIDTH];

    tui_enable_raw_mode();
    tui_clear_screen();

    tui_set_cursor_position(1, 1);

    TUIStyle instructions;
    instructions.style = STYLE_ITALIC;
    instructions.fg_color = FG_BLUE;
    instructions.bg_color = BG_DEFAULT;
    tui_set_style(&instructions);
    printf("Input mode: Move(WASD)  Set(Space)  Submit(Enter)  Random canvas(R)\n");
    tui_reset_style();

    int input_mode = 1;
    fill_canvas(canvas);
    char buf;
    int cursor_x, cursor_y;
    while (input_mode)
    {
        read(STDIN_FILENO, &buf, 1);
        switch (buf)
        {
        case 'w':
            tui_move_cursor_up(1);
            break;
        case 'a':
            tui_move_cursor_back(2);
            break;
        case 's':
            tui_move_cursor_down(1);
            break;
        case 'd':
            tui_move_cursor_forward(2);
            break;
        case 'r':
            fill_random_canvas(canvas);
            input_mode = 0;
            break;
        case 10: // Enter
            input_mode = 0;
            break;
        case ' ':
        {
            tui_get_cursor_position(&cursor_x, &cursor_y);
            if (canvas[AT(cursor_y - 2, (cursor_x - 1) / 2)] == DEAD_CELL)
            {
                canvas[AT(cursor_y - 2, (cursor_x - 1) / 2)] = ALIVE_CELL;
            }
            else
            {
                canvas[AT(cursor_y - 2, (cursor_x - 1) / 2)] = DEAD_CELL;
            }

            tui_set_cursor_position(1, 2);
            print_canvas(canvas);
            tui_set_cursor_position(cursor_x, cursor_y);
            break;
        }
        }
    }

    tui_hide_cursor();
    tui_clear_screen();

    while (1)
    {
        tui_set_cursor_position(1, 1);

        update(canvas);

        print_canvas(canvas);
        usleep(100 * 1000);
    }
    return 0;
}