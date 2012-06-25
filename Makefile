CC      = gcc
# activate MODET because otherwise we would need to recompile libsim.a
CFLAGS  = -DMODET
LDFLAGS = -lbfd -lintl -liberty
GDBBUILDFOLDER = /d/build/gdb-7.4.1

all: emulator

emulator: testarmemul.c
	$(CC) $(CFLAGS) armulmem.c    -c armulmem.o    -I$(GDBBUILDFOLDER)/sim/arm/
	$(CC) $(CFLAGS) testarmemul.c -c testarmemul.o -I$(GDBBUILDFOLDER)/sim/arm/
	gcc -o testarmemul.exe testarmemul.o armulmem.o $(GDBBUILDFOLDER)/sim/arm/libsim.a $(LDFLAGS)
#	testarmemul.exe

disassembler: testarmdisasm.c
	# libopcodes is included directly, because I had to compile a version which included the ARM disassembler.
	$(CC) $(CFLAGS) testarmdisasm.c -o testarmdisasm.exe $(GDBBUILDFOLDER)/opcodes/libopcodes.a $(LDFLAGS)
#	testarmdisasm.exe