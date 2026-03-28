#include "micro_dwm/micro_dwm.h"
#include "micro_lib/micro_lib.h"
#include "micro_term/micro_term.h"
#include "pico/stdlib.h"
#include "lvgl/lvgl.h"
#include <pico/time.h>
#include <src/font/lv_symbol_def.h>
#include <src/misc/lv_timer.h>
#include <stddef.h>
#include <stdlib.h>
#include "spi_dma.h"
#include "screen.h"
#include "board.h"
#include "micro_term.h"
#include "micro_lib.h"
#include "micro_dwm.h"
#include "themes/theme.h"
#include "mouse.h"
#include <math.h>

uint32_t my_tick_get_cb(void) {
    return time_us_32() / 1000;
}

int main()
{
    stdio_init_all();
    init_SPI();
    init_DMA();
    init_display();
    lv_init();
    lv_tick_set_cb(my_tick_get_cb);

    static uint8_t buf1[DRAW_BUF_SIZE];
    static uint8_t buf2[DRAW_BUF_SIZE];
    lv_display_t *disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_buffers(disp, buf1, buf2, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565_SWAPPED);

    micro_dwm_init();
    micro_app_t *iterm = create_micro_term("Tminal");
    micro_change_desktop(1);
    micro_app_t *term = create_micro_term("iTerm");
    micro_set_output(OUT_SCREEN);
    micro_set_focus(iterm);
    micro_puts("~ $ fastfetch\n");
    micro_puts(".-.-.-..-.,-.\n");
    micro_puts("| | | || . < \n");
    micro_puts("`-----'`-'`-'\n");
    micro_puts("CPU : Pico 2\nScreen : ILI9341 320x240\nOS : micro_os\nFont : JetBrains Mono 14\nBest language : C\n");
    micro_puts("~ $ _");
    micro_set_focus(term);

    while (1)
    {
        uint32_t ms_until_next = lv_timer_handler();
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT) {
            if (c >= '0' && c <= '2') {
                int target = c - '0';
                micro_change_desktop(target);
            } else if (c == 'a') create_micro_term("Tminal");
            else if (c == 'c') micro_set_focus(iterm);
            else if (c == 'q') close_last_app();
            else if (c == 't') dwm_theme_toggle();
        }
        if (ms_until_next > 0) {
            uint32_t sleep_time = (ms_until_next > 5) ? 5 : ms_until_next;
            sleep_ms(sleep_time);
        }
    }
    return 0;
}
