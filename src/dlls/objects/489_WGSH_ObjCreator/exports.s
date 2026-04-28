.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGSH_ObjCreator_ctor
.dword WGSH_ObjCreator_dtor

# export table
/*0*/ .dword WGSH_ObjCreator_setup
/*1*/ .dword WGSH_ObjCreator_control
/*2*/ .dword WGSH_ObjCreator_update
/*3*/ .dword WGSH_ObjCreator_print
/*4*/ .dword WGSH_ObjCreator_free
/*5*/ .dword WGSH_ObjCreator_get_model_flags
/*6*/ .dword WGSH_ObjCreator_get_data_size
