# File Updated with AI

CC      = x86_64-elf-g++
LD      = x86_64-elf-ld
OBJCOPY = x86_64-elf-objcopy
ASM     = nasm

CFLAGS = -m32 -ffreestanding -fno-exceptions -fno-rtti -Wall -Wextra -Ikernel -Ikernel/io -Ikernel/terminal -Ikernel/types -Ikernel/maps -Ikernel/helper
LDFLAGS = -m elf_i386 -T kernel/linker.ld

BOOT    = bootloader/boot.bin
OS      = os.bin

KERNEL_ASM_SRCS = kernel/kernel_entry.asm
KERNEL_C_SRCS   = $(wildcard kernel/*.cpp) $(wildcard kernel/**/*.cpp)

KERNEL_ASM_OBJS = $(KERNEL_ASM_SRCS:.asm=.o)
KERNEL_C_OBJS   = $(KERNEL_C_SRCS:.cpp=.o)
KERNEL_OBJS     = $(KERNEL_ASM_OBJS) $(KERNEL_C_OBJS)

KERNEL_ELF = kernel/kernel.elf
KERNEL_BIN = kernel/kernel.bin

.PHONY: all run debug clean

all: $(OS)

$(BOOT): bootloader/boot.asm
	$(ASM) -f bin $< -o $@

%.o: %.asm
	$(ASM) -f elf32 $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

$(OS): $(BOOT) $(KERNEL_BIN)
	cat $^ > $@

run: all
	qemu-system-i386 -drive format=raw,file=$(OS) -display cocoa,zoom-to-fit=on

debug: all
	qemu-system-i386 -drive format=raw,file=$(OS) -s -S -display cocoa,zoom-to-fit=on

clean:
	rm -f $(BOOT) $(KERNEL_OBJS) $(KERNEL_ELF) $(KERNEL_BIN) $(OS)
