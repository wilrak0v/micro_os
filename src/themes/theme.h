#pragma once
#include "lvgl/lvgl.h"

typedef struct {
    lv_color_t bg;
    lv_color_t border_inactive;
    lv_color_t border_active;
    lv_color_t text;
    lv_color_t win_header;
    lv_color_t win_content;
} dwm_theme_t;

extern dwm_theme_t dark_theme;
extern dwm_theme_t light_theme;
extern dwm_theme_t *current_theme;

void init_themes(void);
void dwm_theme_toggle(void);
