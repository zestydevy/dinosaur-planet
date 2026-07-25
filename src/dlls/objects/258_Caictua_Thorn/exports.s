.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Caictua_Thorn_ctor
.dword Caictua_Thorn_dtor

# export table
/*0*/ .dword Caictua_Thorn_setup
/*1*/ .dword Caictua_Thorn_control
/*2*/ .dword Caictua_Thorn_update
/*3*/ .dword Caictua_Thorn_print
/*4*/ .dword Caictua_Thorn_free
/*5*/ .dword Caictua_Thorn_get_model_flags
/*6*/ .dword Caictua_Thorn_get_data_size
