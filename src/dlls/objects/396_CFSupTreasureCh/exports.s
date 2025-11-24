.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFSupTreasureCh_ctor
.dword CFSupTreasureCh_dtor

# export table
/*0*/ .dword CFSupTreasureCh_setup
/*1*/ .dword CFSupTreasureCh_control
/*2*/ .dword CFSupTreasureCh_update
/*3*/ .dword CFSupTreasureCh_print
/*4*/ .dword CFSupTreasureCh_free
/*5*/ .dword CFSupTreasureCh_get_model_flags
/*6*/ .dword CFSupTreasureCh_get_data_size
/*7*/ .dword CFSupTreasureCh_func_700
/*8*/ .dword CFSupTreasureCh_func_718
/*9*/ .dword CFSupTreasureCh_func_7B8
