.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GPSH_ObjCreator_ctor
.dword GPSH_ObjCreator_dtor

# export table
/*0*/ .dword GPSH_ObjCreator_setup
/*1*/ .dword GPSH_ObjCreator_control
/*2*/ .dword GPSH_ObjCreator_update
/*3*/ .dword GPSH_ObjCreator_print
/*4*/ .dword GPSH_ObjCreator_free
/*5*/ .dword GPSH_ObjCreator_get_model_flags
/*6*/ .dword GPSH_ObjCreator_get_data_size
