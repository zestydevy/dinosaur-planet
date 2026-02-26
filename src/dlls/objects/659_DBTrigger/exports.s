.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBTrigger_ctor
.dword DBTrigger_dtor

# export table
/*0*/ .dword DBTrigger_setup
/*1*/ .dword DBTrigger_control
/*2*/ .dword DBTrigger_update
/*3*/ .dword DBTrigger_print
/*4*/ .dword DBTrigger_free
/*5*/ .dword DBTrigger_get_model_flags
/*6*/ .dword DBTrigger_get_data_size
