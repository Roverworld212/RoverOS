CC = gcc
CFLAGS = -w -m32 -c -g -ffreestanding -Wall -I ./Headers
AS = nasm
ASFLAGS = -g -Fdwarf -felf
OBJDIR = ./Compiled/

#Sources
CORE_SRC = $(wildcard ./Kernel/Core/*.c)
LIBS_SRC = $(wildcard ./Libs/*.c)
KERNEL_SRC = $(wildcard ./Kernel/*.c)
MISC_SRC = $(wildcard ./Kernel/Misc/*.c)

#Output
CORE_OUT = $(CORE_SRC:.c=.o)
LIBS_OUT = $(LIBS_SRC:.c=.o)
KERNEL_OUT = $(KERNEL_SRC:.c=.o)
MISC_OUT = $(MISC_SRC:.c=.o)

#Never touch these!

.o:%.c;

all: compile
	qemu-system-x86_64 -no-reboot -d guest_errors,int -cdrom ./rOS.iso -serial file:Out.log -monitor stdio
	rm -f ./*.o --preserve-root
	rm -f ./Compilied/*.o --preserve-root
	rm -f ./Kernel/*.o --preserve-root
	rm -f ./Kernel/Core/*.o --preserve-root
	rm -f ./Kernel/Misc/*.o --preserve-root
	rm -f ./Libs/*.o --preserve-root

gdb: compile
	qemu-system-x86_64 -no-reboot -d guest_errors,int -cdrom ./rOS.iso -s -S -serial file:Out.log -monitor stdio
	rm -f ./*.o --preserve-root
	rm -f ./Compilied/*.o --preserve-root
	rm -f ./Kernel/*.o --preserve-root
	rm -f ./Kernel/Core/*.o --preserve-root
	rm -f ./Kernel/Misc/*.o --preserve-root
	rm -f ./Libs/*.o --preserve-root

update-cfg:
	cp ./grub.cfg ./isodir/boot/grub/grub.cfg
	@echo "Updated"

sym:
	@echo "Getting symbols"
	ld -m elf_i386 -T linker.ld ${OBJDIR}*.o -o ./rOS.o -nostdlib
	objcopy --only-keep-debug ./rOS.o ./Sym/roveros.sym


link:
	@echo "Linking Kernel"
	ld -m elf_i386 -T linker.ld ${OBJDIR}*.o -o rOS.bin -nostdlib
	@echo "Generating ISO"
	grub-file --is-x86-multiboot2 rOS.bin
	mkdir -p ./isodir/boot/grub
	cp ./rOS.bin ./isodir/boot/rOS.bin
	cp ./grub.cfg ./isodir/boot/grub/grub.cfg
	grub-mkrescue -o ./rOS.iso ./isodir

boot:
	@echo "Generating ramdisk"
	gcc ./ramdisk/ramdisk.c -o ./ramdisk/ramdisk.o -I ./Headers
	chmod +x ./ramdisk/ramdisk.o
	./ramdisk/ramdisk.o
	mv ./ramdisk/disk.initrd ./isodir/boot
	@echo "Created ramdisk"
	${AS} ${ASFLAGS} ./Kernel/Boot/boot.asm -o ${OBJDIR}boot.o -I ./Headers
	${AS} ${ASFLAGS} ./Kernel/Boot/idt.asm -o ${OBJDIR}idtb.o -I ./Headers
	${AS} ${ASFLAGS} ./Kernel/Boot/general.asm -o ${OBJDIR}genb.o -I ./Headers

compile: boot $(KERNEL_OUT) $(CORE_OUT) $(LIBS_OUT) $(MISC_OUT) mov sym link

mov:
	mv --force ./Kernel/*.o ${OBJDIR}
	mv --force ./Kernel/Core/*.o ${OBJDIR}
	mv --force ./Libs/*.o ${OBJDIR}
	mv --force ./Kernel/Misc/*.o ${OBJDIR}