#pragma once

#include "micro_lib.h"
#include "lvgl/lvgl.h"

void micro_dwm_init(void);
void micro_dwm_bar_init(lv_obj_t *parent);
void micro_change_desktop(int desktop);
micro_app_t *create_micro_app(const char *title);
