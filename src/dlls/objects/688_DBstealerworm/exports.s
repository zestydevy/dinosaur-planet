.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBStealerWorm_ctor
.dword DBStealerWorm_dtor

# export table
/*0*/ .dword DBStealerWorm_setup
/*1*/ .dword DBStealerWorm_control
/*2*/ .dword DBStealerWorm_update
/*3*/ .dword DBStealerWorm_print
/*4*/ .dword DBStealerWorm_free
/*5*/ .dword DBStealerWorm_get_model_flags
/*6*/ .dword DBStealerWorm_get_data_size
/*7*/ .dword DBStealerWorm_func_6DC
/*8*/ .dword DBStealerWorm_receive_message
