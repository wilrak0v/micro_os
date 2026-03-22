#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "lvgl/lvgl.h"
#include "micro_dwm.h"
#include "micro_lib.h"
#include <src/core/lv_obj_style.h>
#include <src/core/lv_obj_tree.h>
#include <src/widgets/win/lv_win.h>
#include <string.h>
#include <src/core/lv_obj_pos.h>
#include <src/layouts/flex/lv_flex.h>
#include <src/micro_lib/micro_lib.h>
#include <src/misc/lv_area.h>
#include <src/widgets/textarea/lv_textarea.h>


micro_app_t *create_micro_term(const char *title)
{
    micro_app_t *term = create_micro_app(title);
    lv_obj_t *content = lv_win_get_content(term->window);
    lv_obj_set_style_pad_all(content, 0, 0);

    lv_obj_t *ta = lv_textarea_create(content);
    lv_obj_remove_style_all(ta);
    lv_obj_set_style_margin_all(ta, 0, 0);
    lv_obj_set_style_pad_all(ta, 3, 0);
    lv_obj_set_size(ta, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_text_font(ta, &lv_font_montserrat_14, 0);
    lv_textarea_set_one_line(ta, false);
    lv_textarea_set_cursor_click_pos(ta, false);
    lv_obj_clear_flag(ta, LV_OBJ_FLAG_SCROLL_ELASTIC);
    return term;
}

void micro_term_add_char(char c, micro_app_t* term)
{
    //lv_textarea_add_char(textarea, 'c')
    if (term == NULL || term->window == NULL) return;
    lv_obj_t *content = lv_win_get_content(term->window);
    lv_obj_t *ta = lv_obj_get_child(content, 0);
    if (ta == NULL) return;
    lv_textarea_add_char(ta, c);
}
