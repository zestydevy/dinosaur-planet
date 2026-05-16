.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFbarrelanim_ctor
.dword DFbarrelanim_dtor

# export table
/*0*/ .dword DFbarrelanim_setup
/*1*/ .dword DFbarrelanim_control
/*2*/ .dword DFbarrelanim_update
/*3*/ .dword DFbarrelanim_print
/*4*/ .dword DFbarrelanim_free
/*5*/ .dword DFbarrelanim_get_model_flags
/*6*/ .dword DFbarrelanim_get_data_size
