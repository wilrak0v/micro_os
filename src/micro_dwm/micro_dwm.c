#include "lvgl/lvgl.h"
#include "micro_lib.h"
#include "micro_term.h"
#include <src/core/lv_obj.h>
#include <src/core/lv_obj_pos.h>
#include <src/core/lv_obj_style.h>
#include <src/font/lv_symbol_def.h>
#include <src/micro_dwm/micro_dwm.h>
#include <src/micro_lib/micro_lib.h>
#include <src/stdlib/lv_string.h>
#include <src/widgets/label/lv_label.h>
#include <src/widgets/win/lv_win.h>
#include <stdlib.h>

#define MAX_DESKTOP 2

lv_obj_t *status_bar;
lv_obj_t *status_label;
lv_obj_t *tile_container;
int current_desktop = 0;

void create_wallpaper(void)
{
    lv_obj_t *bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bg, 320, 240);
    lv_obj_center(bg);

    lv_obj_set_style_bg_color(bg, lv_color_make(15, 15, 25), 0);
    lv_obj_set_style_bg_grad_color(bg, lv_color_make(5, 5, 10), 0);
    lv_obj_set_style_bg_grad_dir(bg, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_style_radius(bg, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_color_t ui_cyan = lv_color_make(0, 180, 210);
    lv_color_t ui_purple = lv_color_make(130, 50, 200);

    lv_obj_t *glow1 = lv_obj_create(bg);
    lv_obj_set_size(glow1, 200, 200);
    lv_obj_align(glow1, LV_ALIGN_TOP_LEFT, -50, -50);
    lv_obj_set_style_radius(glow1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(glow1, ui_purple, 0);
    lv_obj_set_style_bg_opa(glow1, LV_OPA_20, 0);
    lv_obj_set_style_border_width(glow1, 0, 0);

    static lv_point_precise_t line_points1[] = { {0, 200}, {320, 50} };
    lv_obj_t *line1 = lv_line_create(bg);
    lv_line_set_points(line1, line_points1, 2);
    lv_obj_set_style_line_width(line1, 2, 0);
    lv_obj_set_style_line_color(line1, ui_cyan, 0);
    lv_obj_set_style_line_opa(line1, LV_OPA_30, 0);

    static lv_point_precise_t line_points2[] = { {0, 220}, {320, 70} };
    lv_obj_t * line2 = lv_line_create(bg);
    lv_line_set_points(line2, line_points2, 2);
    lv_obj_set_style_line_width(line2, 1, 0);
    lv_obj_set_style_line_color(line2, ui_purple, 0);
    lv_obj_set_style_line_opa(line2, LV_OPA_40, 0);

    lv_obj_t * arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 250, 250);
    lv_obj_align(arc, LV_ALIGN_RIGHT_MID, 100, 0);
    lv_arc_set_angles(arc, 120, 240);
    lv_arc_set_bg_angles(arc, 120, 240);
    lv_obj_set_style_arc_width(arc, 15, 0);
    lv_obj_set_style_arc_color(arc, ui_cyan, 0);
    lv_obj_set_style_arc_opa(arc, LV_OPA_10, 0);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);

    lv_obj_t * accent = lv_obj_create(bg);
    lv_obj_set_size(accent, 80, 2);
    lv_obj_align(accent, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_color(accent, ui_cyan, 0);
    lv_obj_set_style_bg_grad_color(accent, ui_purple, 0);
    lv_obj_set_style_bg_grad_dir(accent, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(accent, 0, 0);
    lv_obj_set_style_bg_opa(accent, LV_OPA_60, 0);
}

void micro_dwm_init(void)
{
  create_wallpaper();
  micro_dwm_bar_init();
}

void micro_dwm_bar_init(void)
{
  status_bar = lv_obj_create(lv_scr_act());
  lv_obj_set_size(status_bar, LV_PCT(100), 20);
  lv_obj_set_pos(status_bar, 0, 0);

  lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x222222), 0);
  lv_obj_set_style_radius(status_bar, 0, 0);
  lv_obj_set_style_border_width(status_bar, 0, 0);
  lv_obj_set_style_pad_all(status_bar, 2, 0);

  lv_obj_t *desktop_label = lv_label_create(status_bar);
  char buffer[32];
  switch (current_desktop) {
  case -1:
    current_desktop = MAX_DESKTOP;
    break;
  case 0:
    lv_strcpy(buffer, "micro_os | [0]  1  2");
    break;
  case 1:
    lv_strcpy(buffer, "micro_os |  0  [1]  0");
    break;
  case 2:
    lv_strcpy(buffer, "micro_os |  0   1  [0]");
    break;
  default:
    current_desktop = 0;
    break;
  }
  lv_label_set_text(desktop_label, buffer);
  lv_obj_set_style_text_color(desktop_label, lv_color_hex(0xFFFFFF), 0);

  lv_obj_t *icon_label = lv_label_create(status_bar);
  lv_label_set_text(icon_label, LV_SYMBOL_BATTERY_3 " " LV_SYMBOL_WIFI);
  lv_obj_set_x(icon_label, LV_PCT(85));
  lv_obj_set_style_text_color(icon_label, lv_color_hex(0xFFFFFF), 0);
}

micro_app_t *create_micro_app(const char *title) {
  micro_app_t *app = malloc(sizeof(micro_app_t));

  app->window = lv_obj_create(tile_container);
  lv_obj_set_flex_grow(app->window, 1);
  lv_obj_set_flex_align(app->window, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_height(app->window, LV_PCT(100));
  lv_obj_set_scroll_dir(app->window, LV_DIR_NONE);

  lv_obj_set_style_radius(app->window, 0, 0);
  lv_obj_set_style_border_width(app->window, 1, 0);
  lv_obj_set_style_border_color(app->window, lv_color_hex(0x444444), 0);
  lv_obj_set_style_bg_color(app->window, lv_color_hex(0x000000), 0);
  lv_obj_set_style_pad_all(app->window, 0, 0);
  lv_obj_set_style_pad_gap(app->window, 0, 0);

  app->content = lv_textarea_create(app->window);
  lv_obj_set_size(app->content, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(app->content, lv_color_hex(0x000000), 0);
  lv_obj_set_style_border_width(app->content, 0, 0);
  lv_obj_set_style_text_color(app->content, lv_color_hex(0x00FF00), 0);
  lv_obj_set_style_pad_all(app->content, 0, 0);
  lv_obj_set_style_border_width(app->content, 0, 0);
  lv_obj_set_y(app->content, -20);

  app->title = title;
  return app;
}
