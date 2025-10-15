.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SeqObj_ctor
.dword SeqObj_dtor

# export table
/*0*/ .dword SeqObj_setup
/*1*/ .dword SeqObj_control
/*2*/ .dword SeqObj_update
/*3*/ .dword SeqObj_print
/*4*/ .dword SeqObj_free
/*5*/ .dword SeqObj_get_model_flags
/*6*/ .dword SeqObj_get_data_size
