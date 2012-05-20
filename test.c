#include "uart_bitbang.h"
#include "cpu_init.h"
#include "console.h"

int main()
{
    cpu_init();
    uartbb_init();

    // We don't use/enable interrupts
    //eint();

    // At this point MCU expected byte 0x55 received form host
    // to calibrate baud rate.
    uartbb_autodetect_baud();
    putstr("Bit width in clock ticks: 0x");
    puthex16(bit_time);
    newline();

    while (1) {
        uint8_t b = uartbb_rx();
        puthex8(b);
        newline();
    }
}
