.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHqueenearthwalker_ctor
.dword SHqueenearthwalker_dtor

# export table
/*0*/ .dword SHqueenearthwalker_setup
/*1*/ .dword SHqueenearthwalker_control
/*2*/ .dword SHqueenearthwalker_update
/*3*/ .dword SHqueenearthwalker_print
/*4*/ .dword SHqueenearthwalker_free
/*5*/ .dword SHqueenearthwalker_get_model_flags
/*6*/ .dword SHqueenearthwalker_get_state_size
