.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BossDrakorDiamond_ctor
.dword BossDrakorDiamond_dtor

# export table
/*0*/ .dword BossDrakorDiamond_setup
/*1*/ .dword BossDrakorDiamond_control
/*2*/ .dword BossDrakorDiamond_update
/*3*/ .dword BossDrakorDiamond_print
/*4*/ .dword BossDrakorDiamond_free
/*5*/ .dword BossDrakorDiamond_get_model_flags
/*6*/ .dword BossDrakorDiamond_get_data_size
