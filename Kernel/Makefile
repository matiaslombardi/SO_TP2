include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
DRIVERS=$(wildcard drivers/*.c)
INTERRUPTIONS=$(wildcard interruptions/*.c)
UTILITIES=$(wildcard utilities/*.c)
LIBRARIES=$(wildcard libraries/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_DRIVERS=$(DRIVERS:.c=.o)
OBJECTS_INTERRUPTIONS=$(INTERRUPTIONS:.c=.o)
OBJECTS_UTILITIES=$(UTILITIES:.c=.o)
OBJECTS_LIBRARIES=$(LIBRARIES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

#MM=-D BUDDY_MM
MM=-D FREE_LIST_MM

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=
all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_UTILITIES) $(OBJECTS_LIBRARIES)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include $(MM) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o interruptions/*.o utilities/*.o libraries/*.o

.PHONY: all clean
