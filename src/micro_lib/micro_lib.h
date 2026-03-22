#pragma once

#include "lvgl/lvgl.h"

typedef enum {
    OUT_SERIAL,
    OUT_SCREEN,
    OUT_BOTH,
} out_target_t;

typedef struct {
    lv_obj_t *window;
    const char *title;
} micro_app_t;

void micro_set_focus(micro_app_t *app);
void micro_set_output(out_target_t target);
void micro_putchar(char c);
void micro_puts(const char *s);
void micro_printf(const char *format, ...);
