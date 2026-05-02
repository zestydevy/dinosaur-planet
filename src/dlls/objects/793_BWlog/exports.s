.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BWlog_ctor
.dword BWlog_dtor

# export table
/*0*/ .dword BWlog_setup
/*1*/ .dword BWlog_control
/*2*/ .dword BWlog_update
/*3*/ .dword BWlog_print
/*4*/ .dword BWlog_free
/*5*/ .dword BWlog_get_model_flags
/*6*/ .dword BWlog_get_data_size
/*7*/ .dword BWlog_func_950
/*8*/ .dword BWlog_func_9E0
/*9*/ .dword BWlog_get_position
/*10*/ .dword BWlog_func_B48
/*11*/ .dword BWlog_func_C3C
/*12*/ .dword BWlog_func_C4C
/*13*/ .dword BWlog_get_state
/*14*/ .dword BWlog_func_D18
/*15*/ .dword BWlog_func_E2C
/*16*/ .dword BWlog_func_E48
/*17*/ .dword BWlog_func_E70
/*18*/ .dword BWlog_func_E80
/*19*/ .dword BWlog_func_E8C
/*20*/ .dword BWlog_func_E9C
