# File updated with AI / AI Assisted

KERNEL   := bin/kernel.elf
ISO      := bin/eszos.iso
ISO_ROOT := bin/iso_root
LIMINE   := limine
DISK_IMG := bin/disk.img
ROOTFS   := rootfs

CXX := x86_64-elf-g++
LD  := x86_64-elf-ld
ASM := nasm

# ---- Output verbosity ----
V ?= 0
ifeq ($(V),0)
    Q := @
    LOG := @printf '  %-5s %s\n'
else
    Q :=
    LOG := @:
endif

KERNEL_INC_DIRS := $(shell find kernel -type d)
INC_FLAGS := $(addprefix -I , $(KERNEL_INC_DIRS))

CXXFLAGS := \
    -std=c++20 \
    -m64 -march=x86-64 \
    -mcmodel=kernel \
    -mno-red-zone \
    -mno-mmx -mno-sse -mno-sse2 \
    -ffreestanding \
    -fno-stack-protector -fno-stack-check \
    -fno-pie -fno-pic \
    -fno-exceptions -fno-rtti \
    -Wall -Wextra \
    -pipe \
    -O2 \
    -g \
    $(INC_FLAGS)

ASMFLAGS := -f elf64 -g -F dwarf

LDFLAGS := \
    -m elf_x86_64 -nostdlib -static \
    -z max-page-size=0x1000 \
    -T kernel/linker.ld

CPP_SRC := $(shell find kernel -name '*.cpp')
ASM_SRC := $(shell find kernel -name '*.asm')
CPP_OBJ := $(CPP_SRC:%.cpp=bin/obj/%.cpp.o)
ASM_OBJ := $(ASM_SRC:%.asm=bin/obj/%.asm.o)
OBJ := $(CPP_OBJ) $(ASM_OBJ)

QEMU := qemu-system-x86_64

MEM ?= 4G
SMP ?= 2
CPU ?= Westmere

HOST_OS := $(shell uname -s)
ifeq ($(HOST_OS),Darwin)
    ACCEL := hvf:tcg
else
    ACCEL := kvm:tcg
endif

QEMU_BASE := \
    -machine q35,accel=$(ACCEL) \
    -cpu $(CPU) \
    -smp $(SMP) \
    -m $(MEM) \
    -serial stdio

AHCI_DISK := \
    -device ich9-ahci,id=ahci \
    -drive file=$(DISK_IMG),if=none,id=disk0,format=raw \
    -device ide-hd,drive=disk0,bus=ahci.0

IDE_DISK := \
    -drive file=$(DISK_IMG),if=none,id=disk0,format=raw \
    -device ide-hd,drive=disk0,bus=ide.1

DISK_SIZE   ?= 8G
PART_OFFSET := 2048s
FAT_LABEL   := ESZOS

.PHONY: all run run-ide run-debug debug disk disk-files clean clean-disk

all: $(ISO)

bin/obj/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(LOG) CXX $<
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

bin/obj/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	$(LOG) ASM $<
	$(Q)$(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL): $(OBJ) kernel/linker.ld
	@mkdir -p $(dir $@)
	$(LOG) LD $@
	$(Q)$(LD) $(LDFLAGS) $(OBJ) -o $@

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
	$(LOG) ISO $(ISO)
	$(Q)xorriso -as mkisofs -quiet \
	    -b boot/limine/limine-bios-cd.bin \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    --efi-boot boot/limine/limine-uefi-cd.bin \
	    -efi-boot-part --efi-boot-image --protective-msdos-label \
	    $(ISO_ROOT) -o $(ISO) 2>/dev/null
	$(LOG) BIOS $(ISO)
	$(Q)out=$$($(LIMINE)/limine bios-install $(ISO) 2>&1) || { printf '%s\n' "$$out"; exit 1; }

# ---- Disk image ------
# Needs parted + mtools (mformat/mcopy)
$(DISK_IMG):
	@mkdir -p $(dir $@)
	$(LOG) DISK $(DISK_IMG)
	$(Q)qemu-img create -f raw $(DISK_IMG) $(DISK_SIZE) > /dev/null
	$(Q)python3 -c "\
import struct; \
f = open('$(DISK_IMG)', 'r+b'); \
mbr = bytearray(512); \
lba_start = 2048; \
lba_size = $(shell echo $$(($(shell echo $(DISK_SIZE) | sed 's/G//')*1024*1024*2 - 2048))); \
entry = struct.pack('<BBBBBBBBII', 0x80, 0xFE,0xFF,0xFF, 0x0C, 0xFE,0xFF,0xFF, lba_start, lba_size); \
mbr[446:462] = entry; \
mbr[510:512] = bytes([0x55,0xAA]); \
f.write(mbr); \
f.close()"
	$(LOG) FAT32 $(FAT_LABEL)
	$(Q)mformat -i $(DISK_IMG)@@$(PART_OFFSET) -F -v $(FAT_LABEL) ::
	@if [ -d $(ROOTFS) ]; then \
	    printf '  %-5s %s\n' COPY "$(ROOTFS)/ -> image"; \
	    mcopy -s -o -i $(DISK_IMG)@@$(PART_OFFSET) $(ROOTFS)/* :: ; \
	fi

disk-files:
	$(LOG) COPY "$(ROOTFS)/ -> image"
	$(Q)mcopy -s -o -i $(DISK_IMG)@@$(PART_OFFSET) $(ROOTFS)/* ::

disk: clean-disk $(DISK_IMG)

# ---- Run targets ---------------------------------------------------------
run: $(ISO) $(DISK_IMG)
	$(QEMU) $(QEMU_BASE) $(AHCI_DISK) -cdrom $(ISO) -boot d

run-ide: $(ISO) $(DISK_IMG)
	$(QEMU) $(QEMU_BASE) $(IDE_DISK) -cdrom $(ISO) -boot d

run-debug: $(ISO) $(DISK_IMG)
	$(QEMU) $(QEMU_BASE) $(AHCI_DISK) -cdrom $(ISO) -boot d \
	    -no-reboot -no-shutdown -d int,cpu_reset -D bin/qemu.log

debug: $(ISO) $(DISK_IMG)
	$(QEMU) $(QEMU_BASE) $(AHCI_DISK) -cdrom $(ISO) -boot d -S -s

# ---- Cleanup -------------------------------------------------------------
clean:
	rm -rf bin/obj bin/kernel.elf bin/eszos.iso bin/iso_root bin/qemu.log

clean-disk:
	rm -f $(DISK_IMG)
