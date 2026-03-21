/*
 * init_SPI()
 * init_DMA()
 * spi_send_cmd(uint8_t cmd)
 * spi_send_data(uint8_t data)
 * spi_send_data16(uint16_t data)
 * cs_select() / cs_deselect
 */
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "board.h"
#include "spi_dma.h"
#include "screen.h"

uint dma_tx;
dma_channel_config dma_config;

/*
 * SPI Functions
 */

void init_SPI()
{
    // Fixer la vitesse à 30 MHz, parfait pour un ILI9341
    spi_init(SPI_PORT, 50000000);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_init(PIN_DC);
    gpio_init(PIN_RST);

    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_put(PIN_CS, 1);
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_RST, 0);
}

void dma_handler()
{
    // 1. On efface le drapeau d'interruption du DMA
    dma_hw->ints0 = 1u << dma_tx;

    // 2. On désélectionne l'écran (Chip Select High)
    cs_deselect();

    // 3. On prévient LVGL qu'il peut préparer la prochaine frame !
    if (active_disp != NULL) {
        lv_display_flush_ready(active_disp);
    }
}

void init_DMA()
{
    dma_tx = dma_claim_unused_channel(true);
    dma_config = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_8);
    channel_config_set_dreq(&dma_config, spi_get_dreq(SPI_PORT, true));
    channel_config_set_read_increment(&dma_config, true);
    channel_config_set_write_increment(&dma_config, false);

    dma_channel_set_irq0_enabled(dma_tx, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);
}
