.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Textblock_ctor
.dword Textblock_dtor

# export table
/*0*/ .dword Textblock_setup
/*1*/ .dword Textblock_control
/*2*/ .dword Textblock_update
/*3*/ .dword Textblock_print
/*4*/ .dword Textblock_free
/*5*/ .dword Textblock_get_model_flags
/*6*/ .dword Textblock_get_data_size
