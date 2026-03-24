#pragma once

#include "micro_lib.h"
#include <src/micro_lib/micro_lib.h>

#define TERM_SIZE 1024

void set_terms_ptr(int value);
micro_app_t *create_micro_term(const char *title);
void micro_term_add_char(char c, micro_app_t *focus);
