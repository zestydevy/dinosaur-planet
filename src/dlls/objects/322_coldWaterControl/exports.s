.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ColdWaterControl_ctor
.dword ColdWaterControl_dtor

# export table
/*0*/ .dword ColdWaterControl_setup
/*1*/ .dword ColdWaterControl_control
/*2*/ .dword ColdWaterControl_update
/*3*/ .dword ColdWaterControl_print
/*4*/ .dword ColdWaterControl_free
/*5*/ .dword ColdWaterControl_get_model_flags
/*6*/ .dword ColdWaterControl_get_data_size
