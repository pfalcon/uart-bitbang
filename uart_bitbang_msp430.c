#include <msp430.h>
#include "uart_bitbang.h"

#define TXD BIT1
#define RXD BIT2


uint16_t bit_time;
uint16_t half_bit_time;

void uartbb_init()
{
    P1DIR &= ~RXD;
    P1DIR |= TXD;
    // SMCLK, continuous mode
    TACTL = TASSEL_2 + MC_2;
}

void uartbb_autodetect_baud()
{
    uint16_t start, stop;
    while (P1IN & RXD);
    start = TAR;
    while (!(P1IN & RXD));
    stop = TAR;
    bit_time = stop - start;
    half_bit_time = bit_time / 2;
}

void uartbb_tx(uint8_t val)
{
    uint16_t start;
    uint8_t bit_count = 8;

    start = TAR;

    P1OUT &= ~TXD;
    while (TAR - start < bit_time);

    do {
        if (val & 1)
            P1OUT |= TXD;
        else
            P1OUT &= ~TXD;
        start += bit_time;
        while (TAR - start < bit_time);
        val >>= 1;
    } while (--bit_count);

    P1OUT |= TXD;
    start += bit_time;
    while (TAR - start < bit_time);
}

uint8_t uartbb_rx()
{
    uint16_t start;
    uint8_t val;
    uint8_t b = 0, bit_count = 8;

    while (P1IN & RXD);
    start = TAR;
    do val = P1IN; while (TAR - start < half_bit_time);
    start += half_bit_time;

    do {
        do val = P1IN; while (TAR - start < bit_time);
        b >>= 1;
        if (val & RXD)
            b |= 0x80;
        start += bit_time;
    } while (--bit_count);

    return b;
}
