.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFPrisonUncle_ctor
.dword CFPrisonUncle_dtor

# export table
/*0*/ .dword CFPrisonUncle_setup
/*1*/ .dword CFPrisonUncle_control
/*2*/ .dword CFPrisonUncle_update
/*3*/ .dword CFPrisonUncle_print
/*4*/ .dword CFPrisonUncle_free
/*5*/ .dword CFPrisonUncle_get_model_flags
/*6*/ .dword CFPrisonUncle_get_data_size
