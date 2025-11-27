.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sidefoodbag_ctor
.dword sidefoodbag_dtor

# export table
/*0*/ .dword sidefoodbag_setup
/*1*/ .dword sidefoodbag_control
/*2*/ .dword sidefoodbag_update
/*3*/ .dword sidefoodbag_print
/*4*/ .dword sidefoodbag_free
/*5*/ .dword sidefoodbag_get_model_flags
/*6*/ .dword sidefoodbag_get_data_size
/*7*/ .dword sidefoodbag_func_41C
/*8*/ .dword sidefoodbag_func_440
/*9*/ .dword sidefoodbag_func_488
/*10*/ .dword sidefoodbag_func_4DC
/*11*/ .dword sidefoodbag_func_530
/*12*/ .dword sidefoodbag_func_594
