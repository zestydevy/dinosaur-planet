.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMFireFly_ctor
.dword WMFireFly_dtor

# export table
/*0*/ .dword WMFireFly_setup
/*1*/ .dword WMFireFly_control
/*2*/ .dword WMFireFly_update
/*3*/ .dword WMFireFly_print
/*4*/ .dword WMFireFly_free
/*5*/ .dword WMFireFly_get_model_flags
/*6*/ .dword WMFireFly_get_data_size
/*7*/ .dword WMFireFly_set_next_spline_coord
