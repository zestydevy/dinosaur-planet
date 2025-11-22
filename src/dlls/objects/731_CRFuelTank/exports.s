.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CRFuelTank_ctor
.dword CRFuelTank_dtor

# export table
/*0*/ .dword CRFuelTank_setup
/*1*/ .dword CRFuelTank_control
/*2*/ .dword CRFuelTank_update
/*3*/ .dword CRFuelTank_print
/*4*/ .dword CRFuelTank_free
/*5*/ .dword CRFuelTank_get_model_flags
/*6*/ .dword CRFuelTank_get_data_size
