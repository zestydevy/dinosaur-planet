.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCfirecrystalin_ctor
.dword CCfirecrystalin_dtor

# export table
/*0*/ .dword CCfirecrystalin_setup
/*1*/ .dword CCfirecrystalin_control
/*2*/ .dword CCfirecrystalin_update
/*3*/ .dword CCfirecrystalin_print
/*4*/ .dword CCfirecrystalin_free
/*5*/ .dword CCfirecrystalin_get_model_flags
/*6*/ .dword CCfirecrystalin_get_data_size
