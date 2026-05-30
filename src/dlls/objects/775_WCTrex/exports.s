.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTrex_ctor
.dword WCTrex_dtor

# export table
/*0*/ .dword WCTrex_setup
/*1*/ .dword WCTrex_control
/*2*/ .dword WCTrex_update
/*3*/ .dword WCTrex_print
/*4*/ .dword WCTrex_free
/*5*/ .dword WCTrex_get_model_flags
/*6*/ .dword WCTrex_get_data_size
