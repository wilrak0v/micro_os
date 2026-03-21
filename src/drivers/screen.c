#include "../../lvgl/lvgl.h"
#include "board.h"
#include "spi_dma.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

lv_display_t *active_disp = NULL; // Pour garder en mémoire l'écran à notifier

void init_display()
{
    cs_select();

    gpio_put(PIN_RST, 0);
    sleep_ms(500);
    gpio_put(PIN_RST, 1);
    sleep_ms(500);

    // yes this is garbage, but I wanted to break it all down
    // when I was debugging what my initialization issues were.
    spi_send_cmd(0xEF);
    spi_send_data(0x03);
    spi_send_data(0x80);
    spi_send_data(0x02);

    spi_send_cmd(0xCF);
    spi_send_data(0x00);
    spi_send_data(0xC1);
    spi_send_data(0x30);

    spi_send_cmd(0xED);
    spi_send_data(0x64);
    spi_send_data(0x03);
    spi_send_data(0x12);
    spi_send_data(0x81);

    spi_send_cmd(0xE8);
    spi_send_data(0x85);
    spi_send_data(0x00);
    spi_send_data(0x78);

    spi_send_cmd(0xCB);
    spi_send_data(0x39);
    spi_send_data(0x2C);
    spi_send_data(0x00);
    spi_send_data(0x34);
    spi_send_data(0x02);

    spi_send_cmd(0xF7);
    spi_send_data(0x20);

    spi_send_cmd(0xEA);
    spi_send_data(0x00);
    spi_send_data(0x00);

    spi_send_cmd(0xC0);
    spi_send_data(0x23);

    spi_send_cmd(0xC1);
    spi_send_data(0x10);

    spi_send_cmd(0xC5);
    spi_send_data(0x3e);
    spi_send_data(0x28);

    spi_send_cmd(0xC7);
    spi_send_data(0x86);

    spi_send_cmd(0x36);
    spi_send_data(0x28);

    spi_send_cmd(0x3A);
    spi_send_data(0x55);

    spi_send_cmd(0xB1);
    spi_send_data(0x00);
    spi_send_data(0x1F); // 61 Hz
    //send_byte(10); // 119 Hz

    spi_send_cmd(0xB6);
    spi_send_data(0x08);
    spi_send_data(0x82);
    spi_send_data(0x27);

    spi_send_cmd(0xF2);
    spi_send_data(0x00);

    spi_send_cmd(0x26);
    spi_send_data(0x01);

    spi_send_cmd(0xE0);
    spi_send_data(0x0F);
    spi_send_data(0x31);
    spi_send_data(0x2B);
    spi_send_data(0x0C);
    spi_send_data(0x0E);
    spi_send_data(0x08);
    spi_send_data(0x4E);
    spi_send_data(0xF1);
    spi_send_data(0x37);
    spi_send_data(0x07);
    spi_send_data(0x10);
    spi_send_data(0x03);
    spi_send_data(0x0E);
    spi_send_data(0x09);
    spi_send_data(0x00);

    spi_send_cmd(0xE1);
    spi_send_data(0x00);
    spi_send_data(0x0E);
    spi_send_data(0x14);
    spi_send_data(0x03);
    spi_send_data(0x11);
    spi_send_data(0x07);
    spi_send_data(0x31);
    spi_send_data(0xC1);
    spi_send_data(0x48);
    spi_send_data(0x08);
    spi_send_data(0x0F);
    spi_send_data(0x0C);
    spi_send_data(0x31);
    spi_send_data(0x36);
    spi_send_data(0x0F);

    spi_send_cmd(0x11);

    sleep_ms(120);

    spi_send_cmd(0x29);

    sleep_ms(120);

    spi_send_cmd(0x13);
}

void init_drawing()
{
    spi_send_cmd(0x2A);
    spi_send_data16(0);
    spi_send_data16(239);

    spi_send_cmd(0x2B);
    spi_send_data16(0);
    spi_send_data16(319);

    sleep_ms(10);

    spi_send_cmd(0x2C);

    cs_select();
    dc_deselect();
}

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    active_disp = disp;

    spi_send_cmd(0x2A);
    spi_send_data16(area->x1);
    spi_send_data16(area->x2);

    spi_send_cmd(0x2B);
    spi_send_data16(area->y1);
    spi_send_data16(area->y2);

    spi_send_cmd(0x2C);

    uint32_t p_count = lv_area_get_width(area) * lv_area_get_height(area);

    cs_select();
    dc_deselect();

    dma_channel_configure(
        dma_tx,
        &dma_config,
        &spi_get_hw(SPI_PORT)->dr,
        px_map,
        p_count * 2,
        true
    );
}
