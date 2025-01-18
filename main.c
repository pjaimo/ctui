#include "tui.h"
#include <unistd.h>

int main()
{
    // style_t s = {0};
    // s.style = STYLE_BOLD;
    // s.fg_color = FG_BLUE;
    // s.bg_color = BG_MAGENTA;
    // s.rgb_bg = NULL;
    // set_rgb(s.rgb_fg, 81, 233, 82);

    // clear_screen();
    // set_style_rgb(&s);
    // printf("Hello ");
    // reset_style();
    // s.style = STYLE_NORMAL;
    // set_style_rgb(&s);
    // printf("World!\n");
    while (1)
    {
        clear_screen();
        hide_cursor();
        set_cursor_position(1, 1);
        printf("-");
        move_cursor_back(1);
        usleep(500 * 1000);
        printf("\\");
        move_cursor_back(1);
        usleep(500 * 1000);
        printf("|");
        move_cursor_back(1);
        usleep(500 * 1000);
        printf("/");
        move_cursor_back(1);
        usleep(500 * 1000);
    }
    return 0;
}