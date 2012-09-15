#include <stdint.h>

#define putchar(c) uartbb_tx(c)
void puthex8(uint8_t val);
void puthex16(uint16_t val);
void putdec(uint16_t val);
void putstr(char *s);
void newline();
