// Esse arquivo foi criado para incluir a biblioteca no arquivo principal
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

extern void npInit(uint pin);
extern void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
extern void npClear();
extern void npWrite();