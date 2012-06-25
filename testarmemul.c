#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <armdefs.h>
#include <armemu.h>

void
print_processor(ARMul_State* state)
  {
    fprintf(stdout, "pc: 0x%p\tr0: %.8i\tr1: %.8i\tr2: %.8i\tr3: %.8i\n    insn: 0x%.8x\tdec: 0x%.8x load: 0x%.8x\tNextInsn: %i\tpc-val: 0x%.8x\tErr: %i\n",
    	state->Reg[15],
    	ARMul_GetReg(state, state->Mode, 0),
    	ARMul_GetReg(state, state->Mode, 1),
    	ARMul_GetReg(state, state->Mode, 2),
    	ARMul_GetReg(state, state->Mode, 3),
    	state->instr,
    	state->decoded,
    	state->loaded,
    	state->NextInstr,
    	ARMul_ReadWord(state, ARMul_GetPC(state)),
    	state->ErrorCode);
  }

int
main ()
  {
    int i, j;
    ARMword data[4] = { 0xE3A01080, 
    			0xE2810001,
    			0xE1B01001,
    			0x0};
    ARMul_State* state;
    
    
    ARMul_EmulateInit ();
    state = ARMul_NewState ();
    
    //ARMul_SelectProcessor(state, ARM_v5_Prop | ARM_v5e_Prop | ARM_v6_Prop);
    //ARMul_MemoryInit (state, 1<<21); // mem_size copied from gdb armul wrapper
    //ARMul_OSInit (state);
    
    ARMul_SetPC (state, (ARMword) &data);
    
    print_processor(state);
    state->Reg[15] = ARMul_DoInstr(state);
    print_processor(state);
    
    state->NextInstr = RESUME;
    
    state->Reg[15] = ARMul_DoInstr(state);
    print_processor(state);
    //ARMul_DoInstr(state);
    //print_processor(state);
    free(state);
    
    return 0;
  }
