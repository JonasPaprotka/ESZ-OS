# File updated with AI / AI Assited

KERNEL := bin/kernel.elf
ISO := bin/eszos.iso
ISO_ROOT := bin/iso_root
LIMINE := limine

CXX := x86_64-elf-g++
LD := x86_64-elf-ld
ASM := nasm

KERNEL_INC_DIRS := $(shell find kernel -type d)
INC_FLAGS := $(addprefix -I , $(KERNEL_INC_DIRS))

CXXFLAGS := \
    -m64 -march=x86-64 \
    -mcmodel=kernel \
    -mno-red-zone \
    -mno-mmx -mno-sse -mno-sse2 \
    -ffreestanding \
    -fno-stack-protector -fno-stack-check \
    -fno-pie -fno-pic \
    -fno-exceptions -fno-rtti \
    -Wall -Wextra \
    $(INC_FLAGS)

ASMFLAGS := -f elf64

LDFLAGS := \
    -m elf_x86_64 -nostdlib -static \
    -z max-page-size=0x1000 \
    -T kernel/linker.ld

CPP_SRC := $(shell find kernel -name '*.cpp')
ASM_SRC := $(shell find kernel -name '*.asm')
CPP_OBJ := $(CPP_SRC:%.cpp=bin/obj/%.cpp.o)
ASM_OBJ := $(ASM_SRC:%.asm=bin/obj/%.asm.o)
OBJ := $(CPP_OBJ) $(ASM_OBJ)

.PHONY: all run clean

all: $(ISO)

bin/obj/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/obj/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL): $(OBJ) kernel/linker.ld
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

$(ISO): $(KERNEL) limine.conf
	@rm -rf $(ISO_ROOT)
	@mkdir -p $(ISO_ROOT)/boot/limine
	@mkdir -p $(ISO_ROOT)/EFI/BOOT
	@cp $(KERNEL) $(ISO_ROOT)/boot/
	@cp limine.conf $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE)/limine-bios.sys $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE)/limine-bios-cd.bin $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE)/limine-uefi-cd.bin $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	xorriso -as mkisofs \
	    -b boot/limine/limine-bios-cd.bin \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    --efi-boot boot/limine/limine-uefi-cd.bin \
	    -efi-boot-part --efi-boot-image --protective-msdos-label \
	    $(ISO_ROOT) -o $(ISO)
	$(LIMINE)/limine bios-install $(ISO)

# RUNS - DIFF RAM PRESETS
run: $(ISO)
	qemu-system-x86_64 -M q35 -m 512M -cdrom $(ISO) -boot d

run-1: $(ISO)
	qemu-system-x86_64 -M q35 -m 1024M -cdrom $(ISO) -boot d

run-2: $(ISO)
	qemu-system-x86_64 -M q35 -m 2048M -cdrom $(ISO) -boot d

run-4: $(ISO)
	qemu-system-x86_64 -M q35 -m 4096M -cdrom $(ISO) -boot d

run-8: $(ISO)
	qemu-system-x86_64 -M q35 -m 8192M -cdrom $(ISO) -boot d

run-16: $(ISO)
	qemu-system-x86_64 -M q35 -m 16384M -cdrom $(ISO) -boot d

run-32: $(ISO)
	qemu-system-x86_64 -M q35 -m 32768M -cdrom $(ISO) -boot d

run-64: $(ISO)
	qemu-system-x86_64 -M q35 -m 65536M -cdrom $(ISO) -boot d

clean:
	rm -rf bin
