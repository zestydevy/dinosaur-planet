.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_27_ctor
.dword dll_27_dtor

# export table
/*0*/ .dword dll_27_init
/*1*/ .dword dll_27_setup_hits_collider
/*2*/ .dword dll_27_setup_terrain_collider
/*3*/ .dword dll_27_func_1E8
/*4*/ .dword dll_27_func_5A8
/*5*/ .dword dll_27_func_624
/*6*/ .dword dll_27_func_C7C
/*7*/ .dword dll_27_reset
/*8*/ .dword dll_27_func_DF4
