ELF2BIN = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc

OPTIMIZATION = -O0
CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)
MACRO = -D GD32E103V_EVAL
CPPFLAGS = 	$(MACRO) \
			-I ./ \
		   	-I ../../vendor/cmsis/gd32e10x/inc \
			-I ../../vendor/cmsis \
			-I ../../vendor/cmsis/gd32e10x/per/inc \
			-I ../../perlib \
			-I ../../kelib

LINKER_FILE = ../general/link.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER_FILE)

MAIN_SOURCE = ./main.c ./fsm.c ./init.c
SYSTEM_SOURCE = ../../vendor/cmsis/gd32e10x/src/system_gd32e10x.c
STARTUP_SOURCE = ../general/startup.c
CMSIS_SOURCE = 	../../vendor/cmsis/gd32e10x/per/src/gd32e10x_rcu.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_gpio.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_misc.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_fmc.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_usart.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_timer.c
PERLIB_SOURCE = ../../perlib/usart.c \
				../../perlib/gpio.c \
				../../perlib/tim.c \
				../../perlib/rcu.c \
				../../perlib/tim_hd.c
KELIB_SOURCE = ../../kelib/kestring.c

SOURCES = $(MAIN_SOURCE) $(SYSTEM_SOURCE) $(STARTUP_SOURCE) $(CMSIS_SOURCE) $(PERLIB_SOURCE) $(KELIB_SOURCE)

ELF = firmware.elf
BIN = firmware.bin
LOAD_ADDR = 0x08000000

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/gd32e1x.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = ../general/gdb.conf

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


