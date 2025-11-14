.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TexScroll2_ctor
.dword TexScroll2_dtor

# export table
/*0*/ .dword TexScroll2_setup
/*1*/ .dword TexScroll2_control
/*2*/ .dword TexScroll2_update
/*3*/ .dword TexScroll2_print
/*4*/ .dword TexScroll2_free
/*5*/ .dword TexScroll2_get_model_flags
/*6*/ .dword TexScroll2_get_data_size
/*7*/ .dword TexScroll2_change_scroll_speed
