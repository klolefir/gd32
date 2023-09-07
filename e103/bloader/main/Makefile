ELF2BIN = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc

OPTIMIZATION = -O0
CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb -mfloat-abi=hard $(OPTIMIZATION)
MACRO = -D GD32E103V_EVAL #-D VECT_TAB_SRAM
CPPFLAGS = 	$(MACRO) \
			-I ./ \
		   	-I ../../vendor/cmsis/gd32e10x/inc \
			-I ../../vendor/cmsis \
			-I ../../vendor/cmsis/gd32e10x/per/inc \
			-I ../../perlib \
			-I ../../kelib

LINKER_FILE = ../general/link_main.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER_FILE)

MAIN_SOURCE = ./main.c
SYSTEM_SOURCE = ../../vendor/cmsis/gd32e10x/src/system_gd32e10x_main.c
STARTUP_SOURCE = ../general/startup.c
CMSIS_SOURCE = 	../../vendor/cmsis/gd32e10x/per/src/gd32e10x_rcu.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_gpio.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_misc.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_fmc.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_usart.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_timer.c \
				 ../../vendor/cmsis/gd32e10x/per/src/gd32e10x_adc.c
PERLIB_SOURCE = ../../perlib/gpio.c \
				../../perlib/rcu.c \
				../../perlib/systick.c \
				../../perlib/fmc.c \
				../../perlib/adc.c \
				../../perlib/usart.c
KELIB_SOURCE = ../../kelib/kestring.c

SOURCES = $(MAIN_SOURCE) $(SYSTEM_SOURCE) $(STARTUP_SOURCE) $(CMSIS_SOURCE) $(PERLIB_SOURCE) $(KELIB_SOURCE)

ELF = firmware.elf
BIN = firmware.bin
LOAD_ADDR = 0x0800c800

BLOADER_CLI = ../../../../gd32e-bloader/gd32e-bloader-cli/

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/gd32e1x_main.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = ../general/gdb.conf

STFLASH = st-flash

DFU = dfu-util

all: $(ELF)
	cp $(BIN) $(BLOADER_CLI)

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


