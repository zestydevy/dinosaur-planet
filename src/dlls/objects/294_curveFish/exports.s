.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword curveFish_ctor
.dword curveFish_dtor

# export table
/*0*/ .dword curveFish_setup
/*1*/ .dword curveFish_control
/*2*/ .dword curveFish_update
/*3*/ .dword curveFish_print
/*4*/ .dword curveFish_free
/*5*/ .dword curveFish_get_model_flags
/*6*/ .dword curveFish_get_data_size
