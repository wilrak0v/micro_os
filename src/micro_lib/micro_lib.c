#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "micro_lib.h"
#include "micro_term.h"
#include <stdio.h>
#include <stdarg.h>
#include "lvgl/lvgl.h"

static out_target_t current_target = OUT_SERIAL;
static micro_app_t *focus = NULL;

void micro_set_focus(micro_app_t *app)
{
    focus = app;
}

void micro_set_output(out_target_t target)
{
    current_target = target;
}

void micro_putchar(char c)
{
    if (current_target == OUT_SERIAL || current_target == OUT_BOTH)
    {
        putchar(c);
    }

    if (current_target == OUT_SCREEN || current_target == OUT_BOTH)
    {
        if (focus != NULL && focus->window != NULL)
            micro_term_add_char(c, focus);
    }
}

void micro_puts(const char *s)
{
    while (*s)
        micro_putchar(*s++);
}

void micro_printf(const char *format, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, format);

    lv_vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);
    micro_puts(buffer);
}
