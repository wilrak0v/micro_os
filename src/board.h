/* board.h - 20th march 2026
 *
 * This contains the board informations
 */
#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_PORT spi0
#define PIN_MISO 14
#define PIN_CS   13
#define PIN_SCK  6
#define PIN_MOSI 7
#define PIN_DC 15
#define PIN_RST 14
#define PIN_LED 22

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define DRAW_BUF_SIZE (SCREEN_WIDTH * 40 * 2)
