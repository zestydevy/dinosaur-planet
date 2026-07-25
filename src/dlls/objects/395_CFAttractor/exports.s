.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFAttractor_ctor
.dword CFAttractor_dtor

# export table
/*0*/ .dword CFAttractor_setup
/*1*/ .dword CFAttractor_control
/*2*/ .dword CFAttractor_update
/*3*/ .dword CFAttractor_print
/*4*/ .dword CFAttractor_free
/*5*/ .dword CFAttractor_get_model_flags
/*6*/ .dword CFAttractor_get_data_size
/*7*/ .dword CFAttractor_func_190
/*8*/ .dword CFAttractor_func_1B4
