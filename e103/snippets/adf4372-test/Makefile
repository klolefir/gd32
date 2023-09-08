ELF2BIN = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc

OPTIMIZATION = -O0
CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)
MACRO = -D GD32E103V_EVAL #-D VECT_TAB_SRAM strange bug usart dont initializa(usart hang)
CPPFLAGS = 	$(MACRO) \
			-I ./ \
		   	-I ~/projects/gd32/e103/vendor/cmsis/gd32e10x/inc \
			-I ~/projects/gd32/e103/vendor/cmsis \
			-I ~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/inc \
			-I ~/projects/gd32/e103/perlib \
			-I ~/projects/gd32/e103/kelib

LINKER_FILE = ~/projects/gd32/e103/snippets/general/link.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER_FILE)

MAIN_SOURCE = ./main.c ./adf4372.c
SYSTEM_SOURCE = ~/projects/gd32/e103/vendor/cmsis/gd32e10x/src/system_gd32e10x.c
STARTUP_SOURCE = ~/projects/gd32/e103/snippets/general/startup.c
CMSIS_SOURCE = 	~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/src/gd32e10x_rcu.c \
				 ~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/src/gd32e10x_gpio.c \
				 ~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/src/gd32e10x_misc.c \
				 ~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/src/gd32e10x_fmc.c \
				 ~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/src/gd32e10x_usart.c \
				 ~/projects/gd32/e103/vendor/cmsis/gd32e10x/per/src/gd32e10x_spi.c
PERLIB_SOURCE = ~/projects/gd32/e103/perlib/gpio.c \
				~/projects/gd32/e103/perlib/systick.c \
				~/projects/gd32/e103/perlib/usart.c \
				~/projects/gd32/e103/perlib/spi.c
KELIB_SOURCE = ~/projects/gd32/e103/kelib/kestring.c

SOURCES = $(MAIN_SOURCE) $(SYSTEM_SOURCE) $(STARTUP_SOURCE) $(CMSIS_SOURCE) $(PERLIB_SOURCE) $(KELIB_SOURCE)

ELF = firmware.elf
BIN = firmware.bin
LOAD_ADDR = 0x08000000

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/gd32e1x.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = ~/projects/gd32/e103/snippets/general/gdb.conf

STFLASH = st-flash

DFU = dfu-util

all: $(ELF)

$(ELF): $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

$(BIN): $(ELF)
	$(ELF2BIN) -O binary $< $@

stflash: $(BIN)
	$(STFLASH) --reset write $< $(LOAD_ADDR)

dfuflash: $(BIN)
	$(DFU) -a 0 -s $(LOAD_ADDR) -D $<

ocdflash: $(BIN)
	$(GDB) -x $(GDB_CONF)

openocd:
	$(OPENOCD) -f $(OPENOCD_INT) -f $(OPENOCD_TAR)


debug: $(ELF)
	$(GDB) -iex "tar ext :$(OPENOCD_PORT)" $^

clean:
	rm $(ELF) $(BIN)


