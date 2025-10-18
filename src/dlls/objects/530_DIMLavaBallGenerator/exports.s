.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMLavaBallGenerator_ctor
.dword DIMLavaBallGenerator_dtor

# export table
/*0*/ .dword DIMLavaBallGenerator_setup
/*1*/ .dword DIMLavaBallGenerator_control
/*2*/ .dword DIMLavaBallGenerator_update
/*3*/ .dword DIMLavaBallGenerator_print
/*4*/ .dword DIMLavaBallGenerator_free
/*5*/ .dword DIMLavaBallGenerator_get_model_flags
/*6*/ .dword DIMLavaBallGenerator_get_data_size
/*7*/ .dword DIMLavaBallGenerator_func_4FC
/*8*/ .dword DIMLavaBallGenerator_func_59C
