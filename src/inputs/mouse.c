#include "lvgl/lvgl.h"
#include <src/font/lv_symbol_def.h>

lv_indev_t *indev_mouse;
int mouse_x = 0;
int mouse_y = 0;
bool mouse_btn_pressed = false;

void get_mouse_coord(lv_indev_t * indev, lv_indev_data_t * data) {
    data->point.x = (lv_value_precise_t) mouse_x;
    data->point.y = (lv_value_precise_t) mouse_y;
    data->state = (mouse_btn_pressed) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void init_mouse()
{
    indev_mouse = lv_indev_create();
    lv_indev_set_type(indev_mouse, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_mouse, get_mouse_coord);

    lv_obj_t * mouse_cursor = lv_image_create(lv_layer_sys());
    lv_obj_remove_flag(mouse_cursor, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(mouse_cursor, LV_SYMBOL_GPS);
    lv_obj_move_foreground(mouse_cursor);

    lv_indev_set_cursor(indev_mouse, mouse_cursor);
}

void set_mouse_coord(int x, int y, bool pressed)
{
    mouse_x = x;
    mouse_y = y;
    mouse_btn_pressed = pressed;
}

void click_mouse(bool pressed)
{
    mouse_btn_pressed = pressed;
}
