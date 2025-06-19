CC_I386=gcc
CC_X64=gcc
CC_ARM32=arm-linux-gnueabi-gcc
CC_ARM64=aarch64-linux-gnu-gcc

CFLAGS_I386 = -m32 -O0
CFLAGS_X64 = -O0
CFLAGS_ARM32 = -marm -march=armv7-a -O0
CFLAGS_ARM64 = -march=armv8-a -O0

ASFLAGS_I386 = -felf32
ASFLAGS_X64 = -felf64
ASFLAGS_ARM32 = -felf32
ASFLAGS_ARM64 = -felf64

LDFLAGS_I386 = -melf_i386
LDFLAGS_X64 =
LDFLAGS_ARM32 = -melf_arm
LDFLAGS_ARM64 = -melf_aarch64

CC=gcc
AS=nasm
LD=ld

ifeq "$(TARGET)" "TARGET_I386"
CC=$(CC_I386)
CFLAGS=$(CFLAGS_I386)
ASFLAGS=$(ASFLAGS_I386)
LDFLAGS=$(LDFLAGS_I386)
endif

ifeq "$(TARGET)" "TARGET_X86_64"
CC=$(CC_X64)
CFLAGS=$(CFLAGS_X64)
ASFLAGS=$(ASFLAGS_X64)
LDFLAGS=$(LDFLAGS_X64)
endif

ifeq "$(TARGET)" "TARGET_ARM32"
CC=$(CC_ARM32)
CFLAGS=$(CFLAGS_ARM32)
ASFLAGS=$(ASFLAGS_ARM32)
LDFLAGS=$(LDFLAGS_ARM32)
endif

ifeq "$(TARGET)" "TARGET_ARM64"
CC=$(CC_ARM64)
CFLAGS=$(CFLAGS_ARM64)
ASFLAGS=$(ASFLAGS_ARM64)
LDFLAGS=$(LDFLAGS_ARM64)
endif

INCLUDES= -I../taint_include

SRC=src
CSRC=$(patsubst src/%,%,$(patsubst %.c,%,$(wildcard $(SRC)/*.c)))
ASMSRC=$(patsubst src/%,%,$(patsubst %.asm,%,$(wildcard $(SRC)/*.asm)))

all: dirs $(CSRC) $(ASMSRC)

dirs:
	mkdir -p bin
	mkdir -p obj

%:	$(SRC)/%.c
		$(CC) -D$(TARGET) $(CFLAGS) $(INCLUDES) -c $< -o obj/$@.o
		$(CC) -D$(TARGET) $(CFLAGS) $(INCLUDES) obj/$@.o -o bin/$@

%:	$(SRC)/%.asm
		$(AS) -D$(TARGET) $(ASFLAGS) $< -o obj/$@.o
		$(LD) $(LDFLAGS) obj/$@.o -o bin/$@

.PHONY : clean
clean :
	-rm obj/* bin/*
	-rmdir obj/
	-rmdir bin/
