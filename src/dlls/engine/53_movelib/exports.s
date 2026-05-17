.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword movelib_ctor
.dword movelib_dtor

# export table
/*0*/ .dword movelib_func_4B8
/*1*/ .dword movelib_func_B84
/*2*/ .dword movelib_func_BA4
/*3*/ .dword movelib_func_CDC
/*4*/ .dword movelib_func_E9C
/*5*/ .dword movelib_func_B90
/*6*/ .dword movelib_func_1E70
/*7*/ .dword movelib_func_1C0C
/*8*/ .dword movelib_func_1DEC
/*9*/ .dword movelib_func_1CC8
/*10*/ .dword movelib_func_1130
/*11*/ .dword movelib_func_14F4
/*12*/ .dword movelib_get_vtx_world_pos
