#include <tui.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

#define WIDTH 10
#define HEIGHT 20

#define EMPTY_CELL '.'
#define FULL_CELL '#'

pthread_mutex_t lock;
static int piece_index = 0;

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
    int x;
    int y;
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

void fill_board(char board[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            board[y][x] = EMPTY_CELL;
        }
    }
}

void print_board(char board[HEIGHT][WIDTH], char back_board[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (board[y][x] == FULL_CELL || back_board[y][x] == FULL_CELL)
            {
                printf("%c", FULL_CELL);
            }
            else
            {
                printf("%c", EMPTY_CELL);
            }
        }
        printf("\n");
    }
}

void add_piece_to_board(char board[HEIGHT][WIDTH], const Piece *piece)
{
    for (int py = 0; py < piece->rows; py++)
    {
        for (int px = 0; px < piece->cols; px++)
        {
            board[py + piece->y][px + piece->x] = piece->render[py * piece->cols + px];
        }
    }
}

void *manage_input(void *arg)
{
    Piece *piece = (Piece *)arg;
    char buff;
    while (1)
    {

        read(STDIN_FILENO, &buff, 1);

        pthread_mutex_lock(&lock);
        if (buff == 'd' && (piece->x + piece->cols) < WIDTH)
        {
            piece->x++;
        }
        if (buff == 'a' && (piece->x + piece->cols) > 0)
        {
            piece->x--;
        }
        if (buff == 's')
        {
            piece->y++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

char invertedlrender[] = {
    FULL_CELL,
    EMPTY_CELL,
    EMPTY_CELL,
    FULL_CELL,
    FULL_CELL,
    FULL_CELL,
    EMPTY_CELL,
    EMPTY_CELL,
    EMPTY_CELL};

char stickrender[] = {
    EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
    EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
    FULL_CELL, FULL_CELL, FULL_CELL, FULL_CELL,
    EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL};

int main()
{
    signal(SIGINT, signal_handle);
    tui_enable_raw_mode();
    tui_hide_cursor();

    char board[HEIGHT][WIDTH] = {0};
    char freezed_board[HEIGHT][WIDTH] = {0};

    fill_board(board);
    fill_board(freezed_board);

    Piece stick = {.render = stickrender, .rows = 4, .cols = 4, .x = 2, .y = 0};
    // Piece stick = {.render = invertedlrender, .rows = 3, .cols = 3, .x = 2, .y = 0};

    Piece pieces[] = {
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
        stick,
    };
    pthread_t th;
    pthread_create(&th, NULL, manage_input, &pieces[piece_index]);

    while (1)
    {
        tui_clear_screen();
        tui_set_cursor_position(1, 1);
        add_piece_to_board(board, &pieces[piece_index]);
        print_board(board, freezed_board);
        if (pieces[piece_index].y < HEIGHT - (pieces[piece_index].rows / 2) - 1)
        {
            pieces[piece_index].y++;
        }
        else
        {
            add_piece_to_board(freezed_board, &pieces[piece_index]);
            piece_index++;
        }
        // rotate_piece(&stick);
        usleep(400 * 1000);
    }

    pthread_join(th, NULL);

    return 0;
}