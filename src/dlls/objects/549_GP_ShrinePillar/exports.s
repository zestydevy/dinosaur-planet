.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GP_ShrinePillar_ctor
.dword GP_ShrinePillar_dtor

# export table
/*0*/ .dword GP_ShrinePillar_setup
/*1*/ .dword GP_ShrinePillar_control
/*2*/ .dword GP_ShrinePillar_update
/*3*/ .dword GP_ShrinePillar_print
/*4*/ .dword GP_ShrinePillar_free
/*5*/ .dword GP_ShrinePillar_get_model_flags
/*6*/ .dword GP_ShrinePillar_get_data_size
