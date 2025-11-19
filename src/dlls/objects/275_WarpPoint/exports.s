.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WarpPoint_ctor
.dword WarpPoint_dtor

# export table
/*0*/ .dword WarpPoint_setup
/*1*/ .dword WarpPoint_control
/*2*/ .dword WarpPoint_update
/*3*/ .dword WarpPoint_print
/*4*/ .dword WarpPoint_free
/*5*/ .dword WarpPoint_get_model_flags
/*6*/ .dword WarpPoint_get_data_size
