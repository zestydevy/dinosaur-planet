.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBDustGeezer_ctor
.dword DBDustGeezer_dtor

# export table
/*0*/ .dword DBDustGeezer_setup
/*1*/ .dword DBDustGeezer_control
/*2*/ .dword DBDustGeezer_update
/*3*/ .dword DBDustGeezer_print
/*4*/ .dword DBDustGeezer_free
/*5*/ .dword DBDustGeezer_get_model_flags
/*6*/ .dword DBDustGeezer_get_data_size
/*7*/ .dword DBDustGeezer_dug_up
/*8*/ .dword DBDustGeezer_is_in_cooldown
/*9*/ .dword DBDustGeezer_func_870
