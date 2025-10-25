.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_702_ctor
.dword dll_702_dtor

# export table
/*0*/ .dword dll_702_setup
/*1*/ .dword dll_702_control
/*2*/ .dword dll_702_update
/*3*/ .dword dll_702_print
/*4*/ .dword dll_702_free
/*5*/ .dword dll_702_get_model_flags
/*6*/ .dword dll_702_get_data_size
/*7*/ .dword dll_702_get_fsa_anim_state
/*8*/ .dword dll_702_func_B88
