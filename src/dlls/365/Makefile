BUILD_DIR = build
DATA_DIRS := data
ASM_DIRS := $(shell find . -type d)
SRC_DIRS := $(shell find . -type d)

C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
DATA_FILES := $(foreach dir,$(DATA_DIRS),$(wildcard $(dir)/*.bin))

O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
           $(foreach file,$(DATA_FILES),$(BUILD_DIR)/$(file:.bin=.o))

##################### Compiler options #######################

CROSS = mips-linux-gnu-
AS = $(CROSS)as
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy

GCC = $(CROSS)gcc
CC = $(IDO)/cc

DEFINE_CFLAGS = -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DDLL=$(DLL)
INCLUDE_CFLAGS = -I . -I ../../../include
ASFLAGS = -EB -mtune=vr4300 -march=vr4300 -I../../../include -modd-spreg
LDFLAGS = -nostartfiles -nodefaultlibs -r -T ../dll.ld --emit-relocs

OPTFLAGS := -O2 -g3

CC_CFLAGS  = -mips2 -KPIC -w -Xcpluscomm -Wab,-r4300_mul $(DEFINE_FLAGS) $(INCLUDE_CFLAGS)
GCC_CFLAGS = -march=vr4300 -mtune=vr4300 -mfix4300 -fPIC -fno-stack-protector -fno-builtin -fno-common -fsigned-char -std=gnu90 -nostdinc $(DEFINE_CFLAGS) $(INCLUDE_CFLAGS)

######################## Targets #############################

$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(DATA_DIRS),$(shell mkdir -p build/$(dir)))

default: all

export DLL := 365

all: $(BUILD_DIR) $(BUILD_DIR)/$(DLL).dll move_dll

move_dll:
	@cp `find build -name *.dll` ../../../bin/assets/dll
	@rm -rf $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	echo $(C_FILES)
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(DLL).elf: $(O_FILES)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@#$(GCC) $(GCC_CFLAGS) $(OPTFLAGS) -c $^ -o $@
	$(CC) -c $(CC_CFLAGS) $(OPTFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.bin
	$(LD) -r -b binary -o $@ $<

$(BUILD_DIR)/$(DLL).dll: $(BUILD_DIR)/$(DLL).elf
	$(ELF2DLL) $< $@

$(BUILD_DIR)/$(DLL).bin: $(BUILD_DIR)/$(DLL).elf
	$(OBJCOPY) $< $@ -O binary

.PHONY: all clean default
