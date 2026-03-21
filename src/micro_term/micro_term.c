#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "lvgl/lvgl.h"

static lv_obj_t *term_ta;

void micro_term_init(void)
{
    term_ta = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(term_ta, 320, 240);
    lv_textarea_set_text(term_ta, "");
}

void micro_term_add_char(char c)
{
    char buf[2] = {c, '\0'};
    lv_textarea_add_text(term_ta, buf);
}
