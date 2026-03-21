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
    lv_obj_t *parent_cont = term->content;
    lv_obj_set_flex_flow(parent_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_t *ta = lv_textarea_create(parent_cont);
    lv_obj_set_size(ta, lv_pct(100), lv_pct(100));
    lv_textarea_set_cursor_click_pos(ta, false);
    lv_obj_set_style_bg_color(ta, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_color(ta, lv_color_hex(0x00FF00), 0);
    lv_obj_set_style_border_width(ta, 0, 0); // Enlever les bordures moches
    lv_obj_set_style_pad_all(ta, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_top(ta, 0, LV_PART_ANY);
    term->content = ta;
    lv_obj_set_user_data(term->window, term);
    return term;
}

void micro_term_add_char(char c, micro_app_t* term)
{
    if (!term || !term->content) return;
    const char *txt = lv_textarea_get_text(term->content);
    size_t len = strlen(txt);
    if (len > 500)
    {
        lv_textarea_set_cursor_pos(term->content, 0);
        for (int i = 0; i < 200; i++)
            lv_textarea_delete_char_forward(term->content);
        lv_textarea_set_cursor_pos(term->content, LV_TEXTAREA_CURSOR_LAST);
    }
    char buf[2] = {c, '\0'};
    lv_textarea_add_text(term->content, buf);
}
