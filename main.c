#include <stdio.h>
#include <unistd.h>

#define TUI_UNICODE
#include <tui.h>

#define WIDTH 80
#define HEIGHT 20

int main()
{
    tui_hide_cursor();
    tui_create_canvas('.', WIDTH, HEIGHT);
    TUIStyle s = {.style = STYLE_DIM, .bg_color = BG_DEFAULT, .fg_color = FG_YELLOW};
    tui_set_style(&s);
    tui_textbox("Hola Mundo como esta mi gente de barcelona!", (Vector2){.x = 20, .y = 10});
    return 0;
}