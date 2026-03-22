#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "lvgl/lvgl.h"
#include "micro_dwm.h"
#include "micro_lib.h"
#include <src/core/lv_obj_style.h>
#include <string.h>
#include <src/core/lv_obj_pos.h>
#include <src/layouts/flex/lv_flex.h>
#include <src/micro_lib/micro_lib.h>
#include <src/misc/lv_area.h>
#include <src/widgets/textarea/lv_textarea.h>


micro_app_t *create_micro_term(const char *title)
{
    micro_app_t *term = create_micro_app(title);
    return term;
}

void micro_term_add_char(char c, micro_app_t* term)
{
}
