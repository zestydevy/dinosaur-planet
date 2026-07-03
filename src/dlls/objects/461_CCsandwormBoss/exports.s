.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCsandwormBoss_ctor
.dword CCsandwormBoss_dtor

# export table
/*0*/ .dword CCsandwormBoss_setup
/*1*/ .dword CCsandwormBoss_control
/*2*/ .dword CCsandwormBoss_update
/*3*/ .dword CCsandwormBoss_print
/*4*/ .dword CCsandwormBoss_free
/*5*/ .dword CCsandwormBoss_get_model_flags
/*6*/ .dword CCsandwormBoss_get_data_size
