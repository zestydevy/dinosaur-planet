.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBWaterPlant_ctor
.dword DBWaterPlant_dtor

# export table
/*0*/ .dword DBWaterPlant_setup
/*1*/ .dword DBWaterPlant_control
/*2*/ .dword DBWaterPlant_update
/*3*/ .dword DBWaterPlant_print
/*4*/ .dword DBWaterPlant_free
/*5*/ .dword DBWaterPlant_get_model_flags
/*6*/ .dword DBWaterPlant_get_data_size
