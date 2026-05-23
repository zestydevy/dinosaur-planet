.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFbarrelcreator_ctor
.dword DFbarrelcreator_dtor

# export table
/*0*/ .dword DFbarrelcreator_setup
/*1*/ .dword DFbarrelcreator_control
/*2*/ .dword DFbarrelcreator_update
/*3*/ .dword DFbarrelcreator_print
/*4*/ .dword DFbarrelcreator_free
/*5*/ .dword DFbarrelcreator_get_model_flags
/*6*/ .dword DFbarrelcreator_get_data_size
