#include <tui.h>
#include <math.h>
#include <signal.h>
#include <string.h>

#define WIDTH 10
#define HEIGHT 20

void signal_handle(int signal)
{
    (void)signal;

    tui_clear_screen();
    tui_set_cursor_position(1, 1);
    printf("Killed program\n");
    exit(EXIT_SUCCESS);
}

typedef struct
{
    char *render;
    int rows;
    int cols;
} Piece;

void print_piece(Piece *piece)
{
    for (int y = 0; y < piece->rows; y++)
    {
        for (int x = 0; x < piece->cols; x++)
        {
            printf("%c", piece->render[y * piece->cols + x]);
        }
        printf("\n");
    }
}

void rotate_piece(Piece *piece)
{
    char new_piece[piece->rows * piece->cols];
    for (int y = 0; y < piece->rows; y++)
    {
        for (int x = 0; x < piece->cols; x++)
        {
            int y_p = x;
            int x_p = piece->rows - 1 - y;
            new_piece[y_p * piece->cols + x_p] = piece->render[y * piece->cols + x];
        }
    }
    memcpy(piece->render, new_piece, piece->rows * piece->cols * sizeof(char));
}

char invertedlrender[] = {
    ' ', '#', ' ',
    ' ', '#', ' ',
    '#', '#', ' '};

char stickrender[] = {
    ' ', '#', ' ', ' ',
    ' ', '#', ' ', ' ',
    ' ', '#', ' ', ' ',
    ' ', '#', ' ', ' '};

int main()
{
    signal(SIGINT, signal_handle);
    tui_enable_raw_mode();
    tui_hide_cursor();

    Piece stick = {.render = stickrender, .rows = 4, .cols = 4};

    char board[HEIGHT][WIDTH] = {0};
    char freezed_board[HEIGHT][WIDTH] = {0};

    print_piece(&stick);
    rotate_piece(&stick);
    print_piece(&stick);

    return 0;
}