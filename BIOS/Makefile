build:
	make bootloader
	make kernel
	cat output/bootloader.bin output/kernel.bin > output/os.bin
bootloader:
	nasm bootloader/boot.asm -f bin -o output/bootloader.bin
kernel:
	nasm kernel/KernelLoader.asm -f bin -o output/kernel.bin
run:
	qemu-system-x86_64 -drive format=raw,if=floppy,file=output/os.bin
.PHONY: run clean build bootloader kernel
