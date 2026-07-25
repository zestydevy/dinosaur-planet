.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBExpBit_ctor
.dword DBExpBit_dtor

# export table
/*0*/ .dword DBExpBit_setup
/*1*/ .dword DBExpBit_control
/*2*/ .dword DBExpBit_update
/*3*/ .dword DBExpBit_print
/*4*/ .dword DBExpBit_free
/*5*/ .dword DBExpBit_get_model_flags
/*6*/ .dword DBExpBit_get_data_size
