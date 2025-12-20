.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_Galleon_ctor
.dword SB_Galleon_dtor

# export table
/*0*/ .dword SB_Galleon_setup
/*1*/ .dword SB_Galleon_control
/*2*/ .dword SB_Galleon_update
/*3*/ .dword SB_Galleon_print
/*4*/ .dword SB_Galleon_free
/*5*/ .dword SB_Galleon_get_model_flags
/*6*/ .dword SB_Galleon_get_data_size
/*7*/ .dword SB_Galleon_func_5C8
/*8*/ .dword SB_Galleon_func_68C
/*9*/ .dword SB_Galleon_func_69C
/*10*/ .dword SB_Galleon_func_700
/*11*/ .dword SB_Galleon_func_710
