.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TrickyGuard_ctor
.dword TrickyGuard_dtor

# export table
/*0*/ .dword TrickyGuard_setup
/*1*/ .dword TrickyGuard_control
/*2*/ .dword TrickyGuard_update
/*3*/ .dword TrickyGuard_print
/*4*/ .dword TrickyGuard_free
/*5*/ .dword TrickyGuard_get_model_flags
/*6*/ .dword TrickyGuard_get_data_size
