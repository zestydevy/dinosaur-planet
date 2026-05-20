.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DRearthwalk_ctor
.dword DRearthwalk_dtor

# export table
/*0*/ .dword DRearthwalk_setup
/*1*/ .dword DRearthwalk_control
/*2*/ .dword DRearthwalk_update
/*3*/ .dword DRearthwalk_print
/*4*/ .dword DRearthwalk_free
/*5*/ .dword DRearthwalk_get_model_flags
/*6*/ .dword DRearthwalk_get_data_size
/*7*/ .dword DRearthwalk_func_197C
/*8*/ .dword DRearthwalk_func_1990
/*9*/ .dword DRearthwalk_func_19B8
/*10*/ .dword DRearthwalk_func_19DC
/*11*/ .dword DRearthwalk_func_1BB8
/*12*/ .dword DRearthwalk_func_1BE0
/*13*/ .dword DRearthwalk_func_1CA4
/*14*/ .dword DRearthwalk_func_1CB4
/*15*/ .dword DRearthwalk_func_1D5C
/*16*/ .dword DRearthwalk_func_1D84
/*17*/ .dword DRearthwalk_func_1DAC
/*18*/ .dword DRearthwalk_func_1DBC
/*19*/ .dword DRearthwalk_func_1DC8
/*20*/ .dword DRearthwalk_func_3420
