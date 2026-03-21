#pragma once

#include "../../lvgl/lvgl.h"
#include <stdint.h>

extern lv_display_t *active_disp;

void init_display();
void init_drawing();
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
