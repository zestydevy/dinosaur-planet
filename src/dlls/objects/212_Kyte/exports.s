.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Kyte_ctor
.dword Kyte_dtor

# export table
/*0*/ .dword Kyte_obj_Setup
/*1*/ .dword Kyte_obj_Control
/*2*/ .dword Kyte_obj_Update
/*3*/ .dword Kyte_obj_Print
/*4*/ .dword Kyte_obj_Free
/*5*/ .dword Kyte_obj_GetModelFlags
/*6*/ .dword Kyte_obj_GetDataSize
/*7*/ .dword Kyte_func_AB4
/*8*/ .dword Kyte_func_AC4
/*9*/ .dword Kyte_func_ADC
/*10*/ .dword Kyte_func_AEC
/*11*/ .dword Kyte_func_B04
/*12*/ .dword Kyte_func_B24
/*13*/ .dword Kyte_func_B34
/*14*/ .dword Kyte_func_B94
/*15*/ .dword Kyte_func_C04
/*16*/ .dword Kyte_func_C18
/*17*/ .dword Kyte_func_C2C
/*18*/ .dword Kyte_func_C48
/*19*/ .dword Kyte_func_C5C
/*20*/ .dword Kyte_func_C74
/*21*/ .dword Kyte_func_C8C
/*22*/ .dword Kyte_func_C9C
/*23*/ .dword Kyte_func_CDC
/*24*/ .dword Kyte_func_CAC
/*25*/ .dword Kyte_func_CCC
/*26*/ .dword Kyte_func_4040
