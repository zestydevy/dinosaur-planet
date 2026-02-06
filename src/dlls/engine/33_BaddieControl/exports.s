.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BaddieControl_ctor
.dword BaddieControl_dtor

# export table
/*0*/ .dword BaddieControl_ctor
/*1*/ .dword BaddieControl_dtor
/*2*/ .dword BaddieControl_func_4EC
/*3*/ .dword BaddieControl_func_768
/*4*/ .dword BaddieControl_func_278
/*5*/ .dword BaddieControl_func_18
/*6*/ .dword BaddieControl_func_264
/*7*/ .dword BaddieControl_func_884
/*8*/ .dword BaddieControl_func_8A4
/*9*/ .dword BaddieControl_func_148C
/*10*/ .dword BaddieControl_func_E30
/*11*/ .dword BaddieControl_func_ED0
/*12*/ .dword BaddieControl_func_8B4
/*13*/ .dword BaddieControl_func_C88
/*14*/ .dword BaddieControl_func_1FAC
/*15*/ .dword BaddieControl_func_24FC
/*16*/ .dword BaddieControl_func_F60
/*17*/ .dword BaddieControl_func_10F4
/*18*/ .dword BaddieControl_func_15CC
/*19*/ .dword BaddieControl_check_hit
/*20*/ .dword BaddieControl_func_1D88
/*21*/ .dword BaddieControl_setup
/*22*/ .dword BaddieControl_change_weapon
/*23*/ .dword BaddieControl_get_health_ratio
