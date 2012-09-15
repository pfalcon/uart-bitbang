#include "uart_bitbang.h"

void newline()
{
    uartbb_tx('\r');
    uartbb_tx('\n');
}

void puthex4(uint8_t b)
{
    b += '0';
    if (b > '9')
        b += 7;
    uartbb_tx(b);
}

void puthex8(uint8_t b)
{
    puthex4(b >> 4);
    puthex4(b & 0xf);
}

void puthex16(uint16_t v)
{
    puthex8(v >> 8);
    puthex8(v  & 0xff);
}

void putstr(char *s)
{
    while (*s) {
        uartbb_tx(*s++);
    }
}

void putdec(uint16_t v)
{
    char buf[6];
    char *p = buf + 5;
    do {
        *--p = (v % 10) + '0';
        v = v / 10;
    } while (v > 0);
    putstr(p);
}
