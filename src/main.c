#include "pico/stdlib.h"
#include "lvgl/lvgl.h"
#include "spi_dma.h"
#include "screen.h"
#include "board.h"

static lv_obj_t * chart;
static lv_chart_series_t * ser1;

static void system_monitor_timer(lv_timer_t * timer) {
    lv_chart_set_next_value(chart, ser1, rand() % 80 + 10);
}

void lv_demo_micro_os(void) {
    // 1. Style de l'arrière-plan (très "Dark Mode OS")
    lv_obj_set_style_bg_color(lv_scr_act(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_grad_color(lv_scr_act(), lv_color_hex(0x121212), 0);
    lv_obj_set_style_bg_grad_dir(lv_scr_act(), LV_GRAD_DIR_VER, 0);

    // 2. Barre de statut supérieure
    lv_obj_t * status_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(status_bar, LV_PCT(100), 30);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x212121), 0);
    lv_obj_set_style_border_side(status_bar, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_radius(status_bar, 0, 0);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * title = lv_label_create(status_bar);
    lv_label_set_text(title, "MicroOS v1.0 - RP2350");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);

    // 3. Création d'une "Fenêtre" de monitoring
    lv_obj_t * window = lv_obj_create(lv_scr_act());
    lv_obj_set_size(window, 280, 180);
    lv_obj_align(window, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_style_radius(window, 10, 0);
    lv_obj_set_style_clip_corner(window, true, 0);
    lv_obj_set_style_border_width(window, 2, 0);
    lv_obj_set_style_border_color(window, lv_palette_main(LV_PALETTE_BLUE), 0);

    // Titre de la fenêtre
    lv_obj_t * win_label = lv_label_create(window);
    lv_label_set_text(win_label, "CPU Usage (%)");
    lv_obj_align(win_label, LV_ALIGN_TOP_LEFT, 0, -5);

    // 4. Graphique (Chart) pour le monitoring
    chart = lv_chart_create(window);
    lv_obj_set_size(chart, 220, 100);
    lv_obj_align(chart, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(chart, 20);

    // Ajouter une série de données
    ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_CYAN), LV_CHART_AXIS_PRIMARY_Y);

    // 5. Un petit Spinner pour faire "pro"
    lv_obj_t * spinner = lv_spinner_create(window, 1000, 60);
    lv_obj_set_size(spinner, 30, 30);
    lv_obj_align(spinner, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    // 6. Animation d'entrée (La fenêtre pop avec un effet de rebond)
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, window);
    lv_anim_set_values(&a, 0, 180); // On anime la hauteur
    lv_anim_set_time(&a, 800);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_height);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot); // Effet rebond
    lv_anim_start(&a);

    // 7. Lancement du timer pour mettre à jour le graphique toutes les 500ms
    lv_timer_create(system_monitor_timer, 500, NULL);
}

int main()
{
    stdio_init_all();
    init_SPI();
    init_DMA();
    init_display();
    lv_init();

    static uint8_t buf1[DRAW_BUF_SIZE];
    static uint8_t buf2[DRAW_BUF_SIZE];
    lv_display_t *disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_buffers(disp, buf1, buf2, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565_SWAPPED);

    lv_demo_micro_os();

    while (1)
    {
        lv_timer_handler();
        sleep_ms(1);
    }
    return 0;
}
