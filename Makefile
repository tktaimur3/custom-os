C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/include/*.h drivers/include/*.h)
INCLUDES = -Ikernel/include -Idrivers/include
OBJ = ${C_SOURCES:.c=.o}

all: kernel.bin clean

kernel.bin:	boot.o idt.o ${OBJ}
	i686-elf-ld -o $@ -nostdlib $^ -T linker.ld -L. -lgcc

boot.o:
	nasm -felf boot.asm -o boot.o

idt.o:
	nasm -felf idt.asm -o idt.o

%.o : %.c ${HEADERS}
	i686-elf-gcc ${INCLUDES} -ffreestanding -c $< -o $@

clean:
	cat kernel.bin > bootloader.flp
	rm -fr kernel/*.o drivers/*.o *.o *.bin
	cp bootloader.flp /mnt/c/Users/Taimur/Documents/OSStuff/bootloader.flp
	rm bootloader.flp