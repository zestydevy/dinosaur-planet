.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBPlaceHolder_ctor
.dword DBPlaceHolder_dtor

# export table
/*0*/ .dword DBPlaceHolder_setup
/*1*/ .dword DBPlaceHolder_control
/*2*/ .dword DBPlaceHolder_update
/*3*/ .dword DBPlaceHolder_print
/*4*/ .dword DBPlaceHolder_free
/*5*/ .dword DBPlaceHolder_get_model_flags
/*6*/ .dword DBPlaceHolder_get_data_size
