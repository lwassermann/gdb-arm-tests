#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <armdefs.h>
#include <armemu.h>

#include "testarmemul.h"

void
print_processor(ARMul_State* state)
  {
    fprintf(stdout, "pc: 0x%p\tr0: %.8i\tr1: %.8i\tr2: %.8i\tr14(lr): %p\n    CPSR: 0x%.8x\tdec: 0x%.8x load: 0x%.8x\tNextInsn: %i\tsp: 0x%.8x\tErr: %i\n",
    	state->Reg[15],
    	ARMul_GetReg(state, state->Mode, 0),
    	ARMul_GetReg(state, state->Mode, 1),
    	ARMul_GetReg(state, state->Mode, 2),
    	ARMul_GetReg(state, state->Mode, 14),
    	ARMul_GetCPSR(state),
    	state->decoded,
    	state->loaded,
    	state->NextInstr,
    	state->Reg[13],
    	state->EndCondition);
  }

int
main ()
  {
    int i, j;
    ARMword data[10] = { 0xE2810001,
    			0xE1B01001,
    			0xE3A03001, 
    			0xe3530001,
    			0xda00000b, 0xE1A01001,
    			0xE1A01001, 0xE1A01001,
    			3942645761, 0xE3530001};
    
    ARMul_State* state;
    
    ARMul_EmulateInit ();
    state = ARMul_NewState ();
    
    state->MemDataPtr = (unsigned char *) data;
    state->MemSize = 4*10;
    
    minReadAddress = (ARMword) 0x0;
    minWriteAddress = (ARMword) 40;

    //ARMul_SelectProcessor(state, ARM_v5_Prop | ARM_v5e_Prop | ARM_v6_Prop);
    //ARMul_OSInit (state);
    
    ARMul_SetPC (state, (9-1)*4);
    state->Reg[13] = state->MemSize-4;
    
    print_processor(state);
    state->Reg[15] = ARMul_DoInstr(state);
    print_processor(state);
//    printf("%i", (state->Reg[15] == minWriteAddress));
    
    state->Reg[15] = ARMul_DoInstr(state);
    print_processor(state);
    
//    state->Reg[15] = ARMul_DoInstr(state);
//    print_processor(state);
    
    free(state);
    
    return 0;
  }

// adding custom Software Interrupts
unsigned __real_ARMul_OSHandleSWI(ARMul_State*, ARMword);
  
unsigned
__wrap_ARMul_OSHandleSWI (ARMul_State * state, ARMword number)
{
	switch(number)
	  {
		case 0x200000:
			state->Emulate = 0; // STOP
			state->EndCondition = 3; // MemoryAccessViolation
			return 1; //TRUE
	  }
	return __real_ARMul_OSHandleSWI(state, number);
}
