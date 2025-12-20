.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FindKyteObject_ctor
.dword FindKyteObject_dtor

# export table
/*0*/ .dword FindKyteObject_setup
/*1*/ .dword FindKyteObject_control
/*2*/ .dword FindKyteObject_update
/*3*/ .dword FindKyteObject_print
/*4*/ .dword FindKyteObject_free
/*5*/ .dword FindKyteObject_get_model_flags
/*6*/ .dword FindKyteObject_get_data_size
/*7*/ .dword FindKyteObject_func_3D0
/*8*/ .dword FindKyteObject_func_3E4
/*9*/ .dword FindKyteObject_func_460
/*10*/ .dword FindKyteObject_func_478
