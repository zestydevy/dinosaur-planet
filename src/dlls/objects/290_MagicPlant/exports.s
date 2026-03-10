.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MagicPlant_ctor
.dword MagicPlant_dtor

# export table
/*0*/ .dword MagicPlant_setup
/*1*/ .dword MagicPlant_control
/*2*/ .dword MagicPlant_update
/*3*/ .dword MagicPlant_print
/*4*/ .dword MagicPlant_free
/*5*/ .dword MagicPlant_get_model_flags
/*6*/ .dword MagicPlant_get_data_size
