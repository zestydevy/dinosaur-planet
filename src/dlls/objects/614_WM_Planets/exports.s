.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMPlanets_ctor
.dword WMPlanets_dtor

# export table
/*0*/ .dword WMPlanets_setup
/*1*/ .dword WMPlanets_control
/*2*/ .dword WMPlanets_update
/*3*/ .dword WMPlanets_print
/*4*/ .dword WMPlanets_free
/*5*/ .dword WMPlanets_get_model_flags
/*6*/ .dword WMPlanets_get_data_size
