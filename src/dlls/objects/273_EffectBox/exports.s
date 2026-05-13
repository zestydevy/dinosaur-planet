.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword EffectBox_ctor
.dword EffectBox_dtor

# export table
/*0*/ .dword EffectBox_setup
/*1*/ .dword EffectBox_control
/*2*/ .dword EffectBox_update
/*3*/ .dword EffectBox_print
/*4*/ .dword EffectBox_free
/*5*/ .dword EffectBox_get_model_flags
/*6*/ .dword EffectBox_get_data_size
