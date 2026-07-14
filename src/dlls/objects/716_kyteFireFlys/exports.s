.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KyteFireFlys_ctor
.dword KyteFireFlys_dtor

# export table
/*0*/ .dword KyteFireFlys_setup
/*1*/ .dword KyteFireFlys_control
/*2*/ .dword KyteFireFlys_update
/*3*/ .dword KyteFireFlys_print
/*4*/ .dword KyteFireFlys_free
/*5*/ .dword KyteFireFlys_get_model_flags
/*6*/ .dword KyteFireFlys_get_data_size
/*7*/ .dword KyteFireFlys_func_354
/*8*/ .dword KyteFireFlys_func_49C
/*9*/ .dword KyteFireFlys_func_4B4
/*10*/ .dword KyteFireFlys_func_4CC
