#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "lvgl/lvgl.h"
#include "micro_dwm.h"
#include "micro_lib.h"
#include "micro_term.h"
#include "theme.h"
#include <src/core/lv_obj_style.h>
#include <src/core/lv_obj_tree.h>
#include <src/misc/lv_types.h>
#include <src/widgets/win/lv_win.h>
#include <string.h>
#include <src/core/lv_obj_pos.h>
#include <src/layouts/flex/lv_flex.h>
#include <src/micro_lib/micro_lib.h>
#include <src/misc/lv_area.h>
#include <src/widgets/textarea/lv_textarea.h>

#define TERM_SIZE 1024

micro_app_t *create_micro_term(const char *title)
{
    micro_app_t *term = lv_malloc(sizeof(micro_app_t));
    term = create_micro_app(title);
    lv_obj_t *content = lv_win_get_content(term->window);
    lv_obj_set_style_pad_all(content, 0, 0);

    lv_obj_t *ta = lv_textarea_create(content);
    lv_obj_remove_style_all(ta);
    lv_obj_set_scrollbar_mode(ta, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_scroll_dir(ta, LV_DIR_VER);
    lv_obj_set_style_margin_all(ta, 0, 0);
    lv_obj_set_style_pad_all(ta, 3, 0);
    lv_obj_set_size(ta, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_text_font(ta, &lv_font_montserrat_14, 0);
    lv_obj_set_style_bg_color(ta, current_theme->win_header, 0);
    lv_obj_set_style_text_color(ta, current_theme->text, 0);

    return term;
}

void micro_term_add_char(char c, micro_app_t *focus)
{
    //lv_textarea_add_char(, 'c')
    if (focus == NULL || focus->window == NULL) return;
    lv_obj_t *content = lv_win_get_content(focus->window);
    lv_obj_t *ta = lv_obj_get_child(content, 0);
    if (ta == NULL) return;
    lv_textarea_add_char(ta, c);
    lv_obj_scroll_to_view(ta, LV_ANIM_OFF);

    const char * txt = lv_textarea_get_text(ta);
    uint32_t len = lv_strlen(txt);

    if(len > TERM_SIZE) {
        lv_textarea_set_text(ta, txt + (len - TERM_SIZE));
    }
}
