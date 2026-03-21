#include "pico/stdlib.h"
#include "lvgl/lvgl.h"
#include <src/font/lv_symbol_def.h>
#include <stdlib.h>
#include "spi_dma.h"
#include "screen.h"
#include "board.h"

// Variables pour les données dynamiques
static lv_obj_t * temp_label;
static lv_obj_t * humidity_label;
static lv_obj_t * wind_label;

// Timer pour simuler des variations météo
static void weather_update_timer(lv_timer_t * timer) {
    // Simulation de température (18-25°C) avec lv_rand
    int8_t temp = lv_rand(18, 25);
    uint8_t hum = lv_rand(60, 85);
    uint8_t wind = lv_rand(5, 15);

    // Mise à jour des labels avec formatage
    lv_label_set_text_fmt(temp_label, "%d°C", temp);
    lv_label_set_text_fmt(humidity_label, LV_SYMBOL_TINT " %d%%", hum);
    lv_label_set_text_fmt(wind_label, LV_SYMBOL_LOOP " %d km/h", wind);
}

void lv_demo_stylisee_os(void) {
    // 1. Fond d'écran (Un dégradé subtil et sombre)
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x101418), 0);
    lv_obj_set_style_bg_grad_color(lv_scr_act(), lv_color_hex(0x202830), 0);
    lv_obj_set_style_bg_grad_dir(lv_scr_act(), LV_GRAD_DIR_VER, 0);

    // --- BARRE DE STATUS (Simulation OS) ---
    lv_obj_t * status_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(status_bar, LV_PCT(100), 24);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(status_bar, LV_OPA_0, 0); // Transparent
    lv_obj_set_style_border_opa(status_bar, LV_OPA_0, 0);
    lv_obj_set_style_pad_all(status_bar, 0, 0);

    lv_obj_t * clock = lv_label_create(status_bar);
    lv_label_set_text(clock, "14:35");
    lv_obj_set_style_text_color(clock, lv_color_white(), 0);
    lv_obj_align(clock, LV_ALIGN_RIGHT_MID, -10, 0);

    lv_obj_t * batt = lv_label_create(status_bar);
    lv_label_set_text(batt, LV_SYMBOL_BATTERY_3);
    lv_obj_set_style_text_color(batt, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_align(batt, LV_ALIGN_RIGHT_MID, -60, 0);

    // --- CONTENEUR DE L'APPLICATION (Le "Style") ---
    // On prend tout l'écran moins la barre de statut
    lv_obj_t * app_cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(app_cont, LV_PCT(95), LV_PCT(85));
    lv_obj_align(app_cont, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_radius(app_cont, 20, 0);
    lv_obj_set_style_bg_color(app_cont, lv_color_hex(0x2A323A), 0);
    lv_obj_set_style_border_width(app_cont, 0, 0);
    lv_obj_set_style_shadow_width(app_cont, 15, 0);
    lv_obj_set_style_shadow_opa(app_cont, LV_OPA_30, 0);

    // Utilisation du Flex Layout pour tout aligner verticalement proprement
    lv_obj_set_layout(app_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(app_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(app_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(app_cont, 15, 0); // Espace entre les éléments

    // --- CONTENU DE L'APP ---

    // 1. Titre Ville
    lv_obj_t * city = lv_label_create(app_cont);
    lv_label_set_text(city, "Lyon");
    // Utilise une police plus grande si possible (20px)
    lv_obj_set_style_text_font(city, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(city, lv_color_white(), 0);

    // 2. Icône Météo Géante (Simulation avec Symbole)
    // C'est ici qu'une image .png rendrait mieux, mais les symboles font le taf
    lv_obj_t * weather_icon = lv_label_create(app_cont);
    lv_label_set_text(weather_icon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(weather_icon, &lv_font_montserrat_20, 0); // triche, on utilise une font de base
    lv_obj_set_style_text_opa(weather_icon, LV_OPA_80, 0);

    // Pour une icône "géante" sans assets, on triche sur le padding/taille
    lv_obj_set_style_text_color(weather_icon, lv_color_hex(0xA0C0E0), 0); // Bleu pastel

    // 3. Température Principale
    temp_label = lv_label_create(app_cont);
    lv_label_set_text(temp_label, "--°C");
    // Si tu as 'lv_font_montserrat_48' d'activé, mets la ici ! Sinon 20.
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(temp_label, lv_color_white(), 0);
    lv_obj_set_style_margin_top(temp_label, -10, 0); // On recolle un peu à l'icône

    // 4. Bloc Détails (Grid)
    lv_obj_t * details_cont = lv_obj_create(app_cont);
    lv_obj_set_size(details_cont, LV_PCT(100), 50);
    lv_obj_set_style_bg_opa(details_cont, LV_OPA_0, 0);
    lv_obj_set_style_border_width(details_cont, 0, 0);
    lv_obj_set_style_pad_all(details_cont, 0, 0);

    // Flex layout horizontal cette fois
    lv_obj_set_layout(details_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(details_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(details_cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Style commun pour les petits labels
    static lv_style_t style_small;
    lv_style_init(&style_small);
    lv_style_set_text_font(&style_small, &lv_font_montserrat_12);
    lv_style_set_text_color(&style_small, lv_palette_main(LV_PALETTE_GREY));

    humidity_label = lv_label_create(details_cont);
    lv_obj_add_style(humidity_label, &style_small, 0);
    lv_label_set_text(humidity_label, LV_SYMBOL_TINT " --%");

    wind_label = lv_label_create(details_cont);
    lv_obj_add_style(wind_label, &style_small, 0);
    lv_label_set_text(wind_label, LV_SYMBOL_LOOP " -- km/h");

    // --- LOGIQUE (Timer) ---
    // Premier appel manuel pour ne pas attendre le premier tick
    weather_update_timer(NULL);
    // Mise à jour toutes les secondes pour l'effet "démo"
    lv_timer_create(weather_update_timer, 1000, NULL);
}

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

    lv_demo_stylisee_os();
    while (1)
    {
        lv_timer_handler();
        sleep_ms(1);
    }
    return 0;
}
