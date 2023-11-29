#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "version.h"

#include "ws2811.h"

#define ARRAY_SIZE(stuff)       (sizeof(stuff) / sizeof(stuff[0]))

// defaults for cmdline options
#define TARGET_FREQ           WS2811_TARGET_FREQ
#define GPIO_PIN              18
#define DMA                   10
#define STRIP_TYPE            WS2811_STRIP_GRB		// WS2812/SK6812RGB integrated chip+leds

#define WIDTH                 24
#define HEIGHT                1
#define LED_COUNT             (WIDTH * HEIGHT)

// -- X data
#define HMC5983_ADD_DATAX_MSB 0x03
#define HMC5983_ADD_DATAX_LSB 0x04

// -- Y data
#define HMC5983_ADD_DATAY_MSB 0x07
#define HMC5983_ADD_DATAY_LSB 0x08

// -- Z data
#define HMC5983_ADD_DATAZ_MSB 0x05 
#define HMC5983_ADD_DATAZ_LSB 0x06

int HMC5983_Init();
float map(float in, float inMin, float inMax, float outMin, float outMax);
void matrix_render(void);
int rgbToHex(int red, int green, int blue);