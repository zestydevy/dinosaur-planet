.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BossDrakor_ctor
.dword BossDrakor_dtor

# export table
/*0*/ .dword BossDrakor_setup
/*1*/ .dword BossDrakor_control
/*2*/ .dword BossDrakor_update
/*3*/ .dword BossDrakor_print
/*4*/ .dword BossDrakor_free
/*5*/ .dword BossDrakor_get_model_flags
/*6*/ .dword BossDrakor_get_data_size
