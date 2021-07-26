CC = gcc
CFLAGS = -Wall -Wno-implicit-function-declaration -Wextra -ffreestanding -mcmodel=kernel -m64 -fno-pic -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -c -ggdb
AS = nasm
INCLUDE = -I./Headers -I./Headers/FS -I./Headers/Kernel -I./Headers/std -I./Headers/Lib
ASFLAGS = -g -Fdwarf -felf64
OBJDIR = ./Compiled/
LD = ld
LDFLAGS = -m elf_x86_64 -nostdlib -T linker.ld
RAMDISK_FLAGS = -f -d

#Sources
BOOTSTRAP_SRC = $(wildcard ./Bootstrap/*.asm)
KNASM_SRC = $(wildcard ./Kernel/Assembly/*.asm)
KERNEL_SRC = $(wildcard ./Kernel/*.c)
IO_SRC = $(wildcard ./Kernel/io/*.c)
MEM_SRC = $(wildcard ./Kernel/Mem/*.c)
SYS_SRC = $(wildcard ./Kernel/Sys/*.c)
LIBRARIES_SRC = $(wildcard ./Libs/*.c)

#Output
BOOTSTRAP_OUT = $(BOOTSTRAP_SRC:.asm=.o)
KNASM_OUT = $(KNASM_SRC:.asm=.o)
KERNEL_OUT = $(KERNEL_SRC:.c=.o)
IO_OUT = $(IO_SRC:.c=.o)
MEM_OUT = $(MEM_SRC:.c=.o)
SYS_OUT = $(SYS_SRC:.c=.o)
LIBRARIES_OUT = $(LIBRARIES_SRC:.c=.o)

%.o: $(notdir %.asm)
	${AS} ${ASFLAGS} ./$< -o $(addsuffix _nasm.o, $@)

%.o: $(notdir %.c)
	${CC} ${CFLAGS} ${INCLUDE} ./$< -o $(addsuffix _kernel.o, $@)

all: compile
	qemu-system-x86_64 -d cpu_reset -D ./qdlog.log -cdrom ./rOS.iso -serial file:Out.log -monitor stdio

gdb: compile
	qemu-system-x86_64 -d cpu_reset -D ./qdlog.log -cdrom ./rOS.iso -s -S -monitor stdio -serial file:Out.log

bochs: compile
	rm -f ./debug.log --preserve-root
	bochs -f .bochsrc -q

compile: $(BOOTSTRAP_OUT) $(KERNEL_OUT) $(IO_OUT) $(MEM_OUT) $(SYS_OUT) $(KNASM_OUT) $(LIBRARIES_OUT) ramdisk mov
	${LD} ${LDFLAGS} ${OBJDIR}*.o -o ${OBJDIR}rOS.bin
	${LD} ${LDFLAGS} ${OBJDIR}*.o -o ${OBJDIR}rOS.o
	objcopy --only-keep-debug ${OBJDIR}rOS.o ./Sym/rOS.sym
	mkdir -p ./isodir/boot/grub
	cp ${OBJDIR}rOS.bin ./isodir/boot/rOS.bin
	cp ./grub.cfg ./isodir/boot/grub/grub.cfg
	grub-mkrescue -o ./rOS.iso ./isodir
	rm -f ./*.o --preserve-root
	rm -f ${OBJDIR}*.o --preserve-root

clean:
	rm -f ./*.o --preserve-root
	rm -f ${OBJDIR}*.o --preserve-root
	rm -f ./Bootstrap/*.o --preserve-root
	rm -f ./Kernel/*.o --preserve-root
	rm -f ./Kernel/io/*.o --preserve-root
	rm -f ./Kernel/Mem/*.o --preserve-root
	rm -f ./Kernel/Sys/*.o --preserve-root
	rm -f ./Libs/*.o --preserve-root
	rm -f ./ramdisk/*.o --preserve-root

mov:
	mv --force ./Bootstrap/*.o ${OBJDIR}
	mv --force ./Kernel/*.o ${OBJDIR}
	mv --force ./Kernel/Assembly/*.o ${OBJDIR}
	mv --force ./Kernel/io/*.o ${OBJDIR}
	mv --force ./Kernel/Mem/*.o ${OBJDIR}
	mv --force ./Kernel/Sys/*.o ${OBJDIR}
	mv --force ./Libs/*.o ${OBJDIR}
	mv --force ./ramdisk/disk.initrd ./isodir/boot/disk.initrd

ramdisk:
	${CC} -ggdb ./ramdisk/ramdisk.c -o ./ramdisk/ramdisk.o -I./Headers/FS -I./Headers
	objcopy --only-keep-debug ./ramdisk/ramdisk.o ./ramdisk/ramdisk.sym
	./ramdisk/ramdisk.o ${RAMDISK_FLAGS}
	rm -f ./ramdisk/ramdisk.o
	rm -f ./ramdisk/segment.tmp
	rm -f ./ramdisk/fheader.tmp
	rm -f ./ramdisk/nameext.tmp
	rm -f ./ramdisk/fldata.tmp

testinc:
	${AS} -felf64 ./testasm.asm -o ./testasm.o
	hexdump ./testasm.o
	objdump -d --disassembler-options=intel-mnemonic testasm.o
	rm -f ./testasm.o

kvfix:
	sudo chown rover ./kvm.log
	sudo chown rover ./rOS.iso

.PHONY: clean mov ramdisk