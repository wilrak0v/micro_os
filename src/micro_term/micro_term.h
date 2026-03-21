#pragma once

#include "micro_lib.h"
#include <src/micro_lib/micro_lib.h>

micro_app_t *create_micro_term(const char *title);
void micro_term_add_char(char c, micro_app_t *term);
