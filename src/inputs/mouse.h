#pragma once

#include "stdbool.h"

void init_mouse();
void set_mouse_coord(int x, int y, bool pressed);
void click_mouse(bool pressed);
