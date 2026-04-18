.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBSH_Symbol_ctor
.dword DBSH_Symbol_dtor

# export table
/*0*/ .dword DBSH_Symbol_setup
/*1*/ .dword DBSH_Symbol_control
/*2*/ .dword DBSH_Symbol_update
/*3*/ .dword DBSH_Symbol_print
/*4*/ .dword DBSH_Symbol_free
/*5*/ .dword DBSH_Symbol_get_model_flags
/*6*/ .dword DBSH_Symbol_get_data_size
