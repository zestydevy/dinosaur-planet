.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_VConsole_ctor
.dword WL_VConsole_dtor

# export table
/*0*/ .dword WL_VConsole_setup
/*1*/ .dword WL_VConsole_control
/*2*/ .dword WL_VConsole_update
/*3*/ .dword WL_VConsole_print
/*4*/ .dword WL_VConsole_free
/*5*/ .dword WL_VConsole_get_model_flags
/*6*/ .dword WL_VConsole_get_data_size
