GDBBUILDFOLDER = /home/lars/Documents/gsoc/gdb
CC      = gcc
# activate MODET because otherwise we would need to recompile libsim.a
CFLAGS  = -DMODET -g
LDFLAGS = 	-lbfd -liberty
			

all: disassembler emulator

emulator: testarmemul.c
	$(CC) $(CFLAGS) armulmem.c -c -I$(GDBBUILDFOLDER)/sim/arm/
	$(CC) $(CFLAGS) testarmemul.c -c -I$(GDBBUILDFOLDER)/sim/arm/
	$(CC) -o testarmemul.exe testarmemul.o armulmem.o ../gdb/sim/arm/libsim.a -Wl,-wrap,ARMul_OSHandleSWI $(LDFLAGS)
#	./testarmemul.exe

disassembler: testarmdisasm.c
	# libopcodes is included directly, because I had to compile a version which included the ARM disassembler.
	$(CC) $(CFLAGS) testarmdisasm.c -m32 -o testarmdisasm.exe $(GDBBUILDFOLDER)/opcodes/libopcodes.a $(LDFLAGS)
#	./testarmdisasm.exe
