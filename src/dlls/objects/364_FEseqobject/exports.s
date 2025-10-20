.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FEseqobject_ctor
.dword FEseqobject_dtor

# export table
/*0*/ .dword FEseqobject_setup
/*1*/ .dword FEseqobject_control
/*2*/ .dword FEseqobject_update
/*3*/ .dword FEseqobject_print
/*4*/ .dword FEseqobject_free
/*5*/ .dword FEseqobject_get_model_flags
/*6*/ .dword FEseqobject_get_data_size
