# compiler / linker
CC       := avr-gcc
OBJ_COPY := avr-objcopy

# directories
BUILD_DIR := build
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)

HEX := $(BIN_DIR)/kociara.hex
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# compiler flags
CFLAGS := -mmcu=atmega328 -O
OBJCOPYFLAGS := -O ihex

# programmer
PROG := avrdude
PROG_PORT := /dev/ttyACM0
PROGFLAGS := -c arduino -p m328p -P $(PROG_PORT)

.PHONY: all clean flash

all: $(HEX)

$(HEX): $(OBJ) | $(BIN_DIR)
	$(OBJ_COPY) $(OBJCOPYFLAGS) $^ $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) $< 

$(BIN_DIR) $(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

flash:
	sudo $(PROG) $(PROGFLAGS) -U flash:w:$(HEX):i

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

help:
	@echo "help   -   display this help"
	@echo "all    -   (default) build the target and place it in $(BIN_DIR)"
	@echo "flash  -   program the target"

-include $(OBJ:.o=.d)