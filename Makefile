
BUILD_DIR = build
ASM_DIRS := asm asm/os
DATA_DIRS := bin bin/mp3
SRC_DIRS := $(shell find src -type d)

C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
DATA_FILES := $(foreach dir,$(DATA_DIRS),$(wildcard $(dir)/*.bin))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
           $(foreach file,$(DATA_FILES),$(BUILD_DIR)/$(file:.bin=.o)) \

##################### Compiler Options #######################
CROSS = mips-linux-gnu-
AS = $(CROSS)as
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy

#CC         := $(QEMU_IRIX) -L tools/ido7.1_compiler tools/ido7.1_compiler/usr/bin/cc
#CC_OLD     := $(QEMU_IRIX) -L tools/ido5.3_compiler tools/ido5.3_compiler/usr/bin/cc

CC = tools/ido_recomp/linux/5.3/cc
CC_OLD = tools/ido_recomp/linux/7.1/cc

ASFLAGS = -EB -mtune=vr4300 -march=vr4300 -Iinclude -modd-spreg
CFLAGS  = -G 0 -non_shared -Xfullwarn -Xcpluscomm -Wab,-r4300_mul
CFLAGS += -D_LANGUAGE_C -D_FINALROM -DF3DEX_GBI_2 -D_MIPS_SZLONG=32
CFLAGS += -I . -I include
LDFLAGS = -T undefined_syms.txt -T undefined_funcs.txt -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(BUILD_DIR)/dino.map --no-check-sections

OPTFLAGS := -O2 -g3

######################## Targets #############################

$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(DATA_DIRS) $(COMPRESSED_DIRS) $(MAP_DIRS) $(BGM_DIRS),$(shell mkdir -p build/$(dir)))

build/src/os/O1/%.o: OPTFLAGS := -O1
build/src/%.o: CC := python3 tools/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --

default: all

TARGET = dino
LD_SCRIPT = $(TARGET).ld

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).z64 verify

clean:
	rm -rf $(BUILD_DIR)

submodules:
	git submodule update --init --recursive

split:
	rm -rf $(DATA_DIRS) $(ASM_DIRS)
	python3.8 ./tools/splat/split.py --rom baserom.z64 --outdir . splat.yaml

setup: clean submodules split
	
$(BUILD_DIR):
	echo $(C_FILES)
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/$(LD_SCRIPT): $(LD_SCRIPT)
	@mkdir -p $(shell dirname $@)
	cpp -P -DBUILD_DIR=$(BUILD_DIR) -o $@ $<

$(BUILD_DIR)/$(TARGET).elf: $(O_FILES) $(BUILD_DIR)/$(LD_SCRIPT)
	$(LD) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.bin
	$(LD) -r -b binary -o $@ $<

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) $< $@ -O binary

# final z64 updates checksum
$(BUILD_DIR)/$(TARGET).z64: $(BUILD_DIR)/$(TARGET).bin
	@cp $< $@

verify: $(BUILD_DIR)/$(TARGET).z64
	md5sum -c checksum.md5

.PHONY: all clean default split setup
