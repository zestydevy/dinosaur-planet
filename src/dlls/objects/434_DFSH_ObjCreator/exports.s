.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFSH_ObjCreator_ctor
.dword DFSH_ObjCreator_dtor

# export table
/*0*/ .dword DFSH_ObjCreator_setup
/*1*/ .dword DFSH_ObjCreator_control
/*2*/ .dword DFSH_ObjCreator_update
/*3*/ .dword DFSH_ObjCreator_print
/*4*/ .dword DFSH_ObjCreator_free
/*5*/ .dword DFSH_ObjCreator_get_model_flags
/*6*/ .dword DFSH_ObjCreator_get_data_size
