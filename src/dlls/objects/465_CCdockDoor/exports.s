.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_465_ctor
.dword dll_465_dtor

# export table
/*0*/ .dword dll_465_setup
/*1*/ .dword dll_465_control
/*2*/ .dword dll_465_update
/*3*/ .dword dll_465_print
/*4*/ .dword dll_465_free
/*5*/ .dword dll_465_get_model_flags
/*6*/ .dword dll_465_get_data_size
