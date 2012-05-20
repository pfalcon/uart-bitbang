#include <msp430.h>
#include "uart_bitbang.h"

// Allow uart_bitbang_config.h to override port and pins to use for UART
#ifdef USE_CONFIG
#include "uart_bitbang_config.h"
#endif

#ifndef UART_BITBANG_PIN_TXD
#define UART_BITBANG_PORT_RXD_DIR P1DIR
#define UART_BITBANG_PORT_TXD_DIR P1DIR
#define UART_BITBANG_PORT_RXD     P1IN
#define UART_BITBANG_PORT_TXD     P1OUT
#define UART_BITBANG_PIN_TXD      BIT1
#define UART_BITBANG_PIN_RXD      BIT2
#endif


uint16_t bit_time;
uint16_t half_bit_time;

void uartbb_init()
{
#if UART_BITBANG_PORT_RXD_DIR == P2DIR || UART_BITBANG_PORT_TXD_DIR == P2DIR
    // Set (entire) P2 as GPIO
    P2SEL = 0;
#endif
    UART_BITBANG_PORT_RXD_DIR &= ~UART_BITBANG_PIN_RXD;
    UART_BITBANG_PORT_TXD_DIR |= UART_BITBANG_PIN_TXD;
    // SMCLK, continuous mode
    TACTL = TASSEL_2 + MC_2;
}

void uartbb_autodetect_baud()
{
    uint16_t start, stop;
    while (UART_BITBANG_PORT_RXD & UART_BITBANG_PIN_RXD);
    start = TAR;
    while (!(UART_BITBANG_PORT_RXD & UART_BITBANG_PIN_RXD));
    stop = TAR;
    bit_time = stop - start;
    half_bit_time = bit_time / 2;
}

void uartbb_tx(uint8_t val)
{
    uint16_t start;
    uint8_t bit_count = 8;

    start = TAR;

    UART_BITBANG_PORT_TXD &= ~UART_BITBANG_PIN_TXD;
    while (TAR - start < bit_time);

    do {
        if (val & 1)
            UART_BITBANG_PORT_TXD |= UART_BITBANG_PIN_TXD;
        else
            UART_BITBANG_PORT_TXD &= ~UART_BITBANG_PIN_TXD;
        start += bit_time;
        while (TAR - start < bit_time);
        val >>= 1;
    } while (--bit_count);

    UART_BITBANG_PORT_TXD |= UART_BITBANG_PIN_TXD;
    start += bit_time;
    while (TAR - start < bit_time);
}

uint8_t uartbb_rx()
{
    uint16_t start;
    uint8_t val;
    uint8_t b = 0, bit_count = 8;

    while (UART_BITBANG_PORT_RXD & UART_BITBANG_PIN_RXD);
    start = TAR;
    do val = UART_BITBANG_PORT_RXD; while (TAR - start < half_bit_time);
    start += half_bit_time;

    do {
        do val = UART_BITBANG_PORT_RXD; while (TAR - start < bit_time);
        b >>= 1;
        if (val & UART_BITBANG_PIN_RXD)
            b |= 0x80;
        start += bit_time;
    } while (--bit_count);

    return b;
}
