BOOT = bootloader/boot.bin
KERNEL = kernel/kernel.bin
OS = os.bin

all: $(OS)

$(BOOT): bootloader/boot.asm
	nasm -f bin $< -o $@

$(KERNEL): kernel/kernel_entry.asm kernel/kernel.c kernel/vga.c kernel/io.c kernel/pic.c kernel/idt.c kernel/keyboard.c
	nasm -f elf32 kernel/kernel_entry.asm -o kernel/entry.o
	x86_64-elf-gcc -m32 -ffreestanding -c kernel/kernel.c -o kernel/kernel.o
	x86_64-elf-gcc -m32 -ffreestanding -c kernel/vga.c -o kernel/vga.o
	x86_64-elf-gcc -m32 -ffreestanding -c kernel/io.c -o kernel/io.o
	x86_64-elf-gcc -m32 -ffreestanding -c kernel/pic.c -o kernel/pic.o
	x86_64-elf-gcc -m32 -ffreestanding -c kernel/idt.c -o kernel/idt.o
	x86_64-elf-gcc -m32 -ffreestanding -c kernel/keyboard.c -o kernel/keyboard.o
	x86_64-elf-ld -m elf_i386 -T kernel/linker.ld -o kernel/kernel.elf kernel/entry.o kernel/kernel.o kernel/vga.o kernel/io.o kernel/pic.o kernel/idt.o kernel/keyboard.o
	x86_64-elf-objcopy -O binary kernel/kernel.elf kernel/kernel.bin

$(OS): $(BOOT) $(KERNEL)
	cat $(BOOT) $(KERNEL) > $(OS)

run: all
	qemu-system-i386 -drive format=raw,file=$(OS) -display cocoa,zoom-to-fit=on

debug: all
	qemu-system-i386 -drive format=raw,file=$(OS) -s -S -display cocoa,zoom-to-fit=on

clean:
	rm -f bootloader/boot.bin kernel/entry.o kernel/kernel.o kernel/vga.o kernel/io.o kernel/pic.o kernel/idt.o kernel/keyboard.o kernel/kernel.elf $(KERNEL) $(OS)

.PHONY: all run debug clean