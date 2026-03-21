#include "micro_lib/micro_lib.h"
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

uint32_t my_tick_get_cb(void) {
    return time_us_32() / 1000; // Renvoie le temps en millisecondes
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
    micro_set_output(OUT_SERIAL);

    while (1)
    {
        uint32_t ms_until_next = lv_timer_handler();
        micro_putchar('W');
        if (ms_until_next > 0) {
            uint32_t sleep_time = (ms_until_next > 5) ? 5 : ms_until_next;
            sleep_ms(sleep_time);
        }
    }
    return 0;
}
