/* spi_dma.h - wilrak0v
 *
 */

#pragma once

#include <stdint.h>
#include "hardware/dma.h"
#include "board.h"

extern uint dma_tx;
extern dma_channel_config dma_config;

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 0);
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop");
}

static inline void dc_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_DC, 0);
    asm volatile("nop \n nop \n nop");
}

static inline void dc_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_DC, 1);
    asm volatile("nop \n nop \n nop");
}


// Send commands
static void inline spi_send_cmd(uint8_t cmd)
{
    dc_select();
    cs_select();
    spi_write_blocking(SPI_PORT, &cmd, 1);
    dc_deselect();
    cs_deselect();
}

static void inline spi_send_data(uint8_t data)
{
    cs_select();
    dc_deselect();
    spi_write_blocking(SPI_PORT, &data, 1);
    cs_deselect();
}

static void inline spi_send_data16(uint16_t data)
{
    cs_select();
    dc_deselect();
    uint8_t shortBuffer[2];
    shortBuffer[0] = (uint8_t) (data >> 8);
    shortBuffer[1] = (uint8_t) data;
    spi_write_blocking(SPI_PORT, shortBuffer, 2);
    cs_deselect();
}

void init_SPI();
void init_DMA();
