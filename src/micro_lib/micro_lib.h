#pragma once

#include "lvgl/lvgl.h"

typedef enum {
    OUT_SERIAL,
    OUT_SCREEN,
    OUT_BOTH,
} out_target_t;

typedef struct {
    lv_obj_t *window;
    lv_obj_t *content;
    const char *title;
    void (*on_close)(void);
} micro_app_t;

void micro_io_init(void);
void micro_set_output(out_target_t target);
int micro_putchar(char c);
int micro_puts(const char *s);
void micro_printf(const char *format, ...);
