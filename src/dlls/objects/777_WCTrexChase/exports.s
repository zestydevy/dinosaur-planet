.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTrexChase_ctor
.dword WCTrexChase_dtor

# export table
/*0*/ .dword WCTrexChase_setup
/*1*/ .dword WCTrexChase_control
/*2*/ .dword WCTrexChase_update
/*3*/ .dword WCTrexChase_print
/*4*/ .dword WCTrexChase_free
/*5*/ .dword WCTrexChase_get_model_flags
/*6*/ .dword WCTrexChase_get_data_size
