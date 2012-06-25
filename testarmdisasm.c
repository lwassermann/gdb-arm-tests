#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <bfd.h> //is also done in dis-asm.h

#include <dis-asm.h>

int
my_fprintf(void* stream, const char * format, ...)
  {
    va_list arg;

    va_start(arg,format);
    vfprintf(stdout, format, arg);
    return 0;
  }

int
main ()
  {
    char data[8] = {0xE3, 0xA0, 0x10, 0x80, 
    		    0xE2, 0x81, 0x00, 0x01};
    bfd_byte* buf = (bfd_byte*) &data[0];
    
    disassemble_info* c = (disassemble_info*) calloc(1, sizeof(disassemble_info));
    // void init_disassemble_info (struct disassemble_info *dinfo, void *stream, fprintf_ftype fprintf_func)
    init_disassemble_info ( c, stdout, my_fprintf);
    
    //c->application_data
    //c->memory_error_func
    
    // set architecture
    c->arch = bfd_arch_arm;
    // set the specific machine: unknown allows all instructions in the libraries database
    c->mach = bfd_mach_arm_unknown;
    
    // should set the disassembler field of c to the right function
    disassemble_init_for_target( c );
    // given a bfd, the disassembler can find the arch by itself.
    //disassemble = disassembler( c );
    
    c->buffer_vma = 0;
    c->buffer = buf;
    c->buffer_length = 8;
    
    // while-loop for calling single instruction decoding:
    unsigned int count = 0;
    size_t pos = 0;
    size_t length = c->buffer_length;
    size_t max_pos = c->buffer_vma+length;
    
    while(pos < max_pos)
      {
      	//disassembler-function: print_insn_big_arm
      	//other possible functions are listed in opcodes/dissassemble.c
      	unsigned int size = print_insn_big_arm((bfd_vma) pos, c);
      	pos += size;
      	count++;
      	fprintf(stdout, "\n");
      }
    return 0;
  }
