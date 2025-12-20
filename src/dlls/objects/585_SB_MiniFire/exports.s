.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_MiniFire_ctor
.dword SB_MiniFire_dtor

# export table
/*0*/ .dword SB_MiniFire_setup
/*1*/ .dword SB_MiniFire_control
/*2*/ .dword SB_MiniFire_update
/*3*/ .dword SB_MiniFire_print
/*4*/ .dword SB_MiniFire_free
/*5*/ .dword SB_MiniFire_get_model_flags
/*6*/ .dword SB_MiniFire_get_data_size
