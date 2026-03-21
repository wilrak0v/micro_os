#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "micro_lib.h"
#include "micro_term.h"
#include <stdio.h>

static out_target_t current_target = OUT_SERIAL;

void micro_set_output(out_target_t target)
{
    current_target = target;
}

int micro_putchar(char c)
{
    if (current_target == OUT_SERIAL || current_target == OUT_BOTH)
    {
        putchar(c);
    }

    if (current_target == OUT_SCREEN || current_target == OUT_BOTH)
    {
        micro_term_add_char(c);
    }
}
