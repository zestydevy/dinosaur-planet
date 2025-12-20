.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword AnimTest_ctor
.dword AnimTest_dtor

# export table
/*0*/ .dword AnimTest_setup
/*1*/ .dword AnimTest_control
/*2*/ .dword AnimTest_update
/*3*/ .dword AnimTest_print
/*4*/ .dword AnimTest_free
/*5*/ .dword AnimTest_get_model_flags
/*6*/ .dword AnimTest_get_data_size
