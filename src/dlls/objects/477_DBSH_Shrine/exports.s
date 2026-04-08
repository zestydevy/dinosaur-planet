.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBSHshrine_ctor
.dword DBSHshrine_dtor

# export table
/*0*/ .dword DBSHshrine_setup
/*1*/ .dword DBSHshrine_control
/*2*/ .dword DBSHshrine_update
/*3*/ .dword DBSHshrine_print
/*4*/ .dword DBSHshrine_free
/*5*/ .dword DBSHshrine_get_model_flags
/*6*/ .dword DBSHshrine_get_data_size
