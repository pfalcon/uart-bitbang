CC = msp430-gcc
CFLAGS=-Os -g -mmcu=msp430g2231
OBJDUMP = msp430-objdump

TARGET = uart_bitbang
OBJS = test.o cpu_init_msp430.o uart_bitbang_msp430.o console.o

all: $(TARGET)

$(TARGET): $(OBJS)

program: $(TARGET)
	mspdebug rf2500 "prog $^"

disasm: $(TARGET)
	$(OBJDUMP) -dSt $^ >$^.disasm

clean:
	rm -rf *.o
