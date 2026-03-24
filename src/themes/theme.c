#include "theme.h"

dwm_theme_t dark_theme;
dwm_theme_t light_theme;
dwm_theme_t *current_theme = &dark_theme;

void init_themes()
{
    dark_theme = (dwm_theme_t){
        .bg = lv_color_hex(0x1A1A1A),
        .border_inactive = lv_color_hex(0x2F3542),
        .border_active = lv_color_hex(0x0000FF),
        .text = lv_color_hex(0xE3E3E3),
        .win_header = lv_color_hex(0x252526),
        .win_content = lv_color_hex(0x1E1E1E),
    };

    light_theme = (dwm_theme_t){
        .bg = lv_color_hex(0xF0F0F0),
        .border_inactive = lv_color_hex(0xD1D1D1),
        .border_active = lv_color_hex(0x007AFF),
        .text = lv_color_hex(0x111827),
        .win_header = lv_color_hex(0xE5E7EB),
        .win_content = lv_color_hex(0xF3F4F6),
    };
}

void dwm_theme_toggle()
{
    if (current_theme == &dark_theme) current_theme = &light_theme;
    else current_theme = &dark_theme;
}
