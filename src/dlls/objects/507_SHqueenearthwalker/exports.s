.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHqueenearthwalker_ctor
.dword SHqueenearthwalker_dtor

# export table
/*0*/ .dword SHqueenearthwalker_create
/*1*/ .dword SHqueenearthwalker_update
/*2*/ .dword SHqueenearthwalker_func_464
/*3*/ .dword SHqueenearthwalker_draw
/*4*/ .dword SHqueenearthwalker_destroy
/*5*/ .dword SHqueenearthwalker_get_setup_flags
/*6*/ .dword SHqueenearthwalker_get_state_size
