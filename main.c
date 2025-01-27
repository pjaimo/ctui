#define TUI_UNICODE
#include "tui.h"

#include <unistd.h>

#define WIDTH 80
#define HEIGHT 20

int main()
{
    hide_cursor();
    // while (1)
    // {
    //     tui_spinner(DOTS);
    // }
    TUIRect r;
    r.height = 5;
    r.width = 15;
    r.pos.x = 3;
    r.pos.y = 3;
    r.fill = ' ';

    TUIStyle s;
    s.style = STYLE_NORMAL;
    s.fg_color = FG_RED;
    s.bg_color = BG_DEFAULT;
    tui_create_canvas('.', WIDTH, HEIGHT);
    set_style(&s);

    tui_add_rect(&r);
    while (1)
    {

        sleep(1);
    }
    return 0;
}