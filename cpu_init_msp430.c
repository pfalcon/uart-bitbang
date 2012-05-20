#include "cpu_init.h"
#include <msp430.h>

void cpu_init()
{
    // Disable watchdog
    WDTCTL = WDTPW | WDTHOLD;

    // Use precalibrated 1MHz frequency
    //BCSCTL1 = CALBC1_1MHZ;
    //DCOCTL = CALDCO_1MHZ;

    // Use high uncalibrated frequency for higher baud rate
    BCSCTL1 = 15 | XT2OFF;
    DCOCTL = 0;
}
