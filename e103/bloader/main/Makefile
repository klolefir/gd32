ELF2BIN = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc

USR = $(USER)
GD32_PATH = /home/$(USR)/projects/gd32/e103/
PERLIB_PATH = $(GD32_PATH)/perlib
KELIB_PATH = $(GD32_PATH)/kelib
VENDOR_PATH = $(GD32_PATH)/vendor/cmsis
GD32LIB_PATH = $(VENDOR_PATH)/gd32e10x/per
OPTIMIZATION = -Os

CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb -mfloat-abi=hard $(OPTIMIZATION)

MACRO = -D GD32E103V_EVAL
MAIN_INC 	=	-I ./
VENDOR_INC 	= 	-I $(VENDOR_PATH)/gd32e10x/inc \
				-I $(GD32LIB_PATH)/inc \
				-I $(VENDOR_PATH)
PERLIB_INC 	= 	-I $(PERLIB_PATH)
KELIB_INC 	= 	-I $(KELIB_PATH)

CPPFLAGS = 	$(MACRO) $(MAIN_INC) $(VENDOR_INC) $(PERLIB_INC) $(KELIB_INC)

LINKER_FILE = $(GD32_PATH)/bloader/general/link_main.ld 
#LINKER_FILE = $(GD32_PATH)/bloader/general/link.ld 
LDFLAGS = -Wall -g -nostdlib -T $(LINKER_FILE)

MAIN_SRC 	=	./main.c
SYSTEM_SRC 	= 	$(VENDOR_PATH)/gd32e10x/src/system_gd32e10x_main.c
#SYSTEM_SRC 	= 	$(VENDOR_PATH)/gd32e10x/src/system_gd32e10x.c
STARTUP_SRC = 	$(GD32_PATH)/snippets/general/startup.c
VENDOR_SRC 	= 	$(GD32LIB_PATH)/src/gd32e10x_rcu.c \
				$(GD32LIB_PATH)/src/gd32e10x_gpio.c \
				$(GD32LIB_PATH)/src/gd32e10x_misc.c \
				$(GD32LIB_PATH)/src/gd32e10x_fmc.c \
				$(GD32LIB_PATH)/src/gd32e10x_usart.c \
				$(GD32LIB_PATH)/src/gd32e10x_timer.c \
				$(GD32LIB_PATH)/src/gd32e10x_dma.c \
				$(GD32LIB_PATH)/src/gd32e10x_adc.c
PERLIB_SRC = 	$(PERLIB_PATH)/usart.c \
				$(PERLIB_PATH)/gpio.c \
				$(PERLIB_PATH)/tim.c \
				$(PERLIB_PATH)/rcu.c \
				$(PERLIB_PATH)/tim_hd.c \
				$(PERLIB_PATH)/fmc.c \
				$(PERLIB_PATH)/adc.c \
				$(PERLIB_PATH)/dma.c \
				$(PERLIB_PATH)/systick.c
KELIB_SRC 	= 	$(KELIB_PATH)/kestring.c

SOURCES = $(MAIN_SRC) $(SYSTEM_SRC) $(STARTUP_SRC) $(VENDOR_SRC) $(PERLIB_SRC) $(KELIB_SRC)

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
