#include <stdint.h>

void    uartbb_init();
void    uartbb_autodetect_baud();
uint8_t uartbb_rx();
void    uartbb_tx(uint8_t c);

extern uint16_t bit_time;
