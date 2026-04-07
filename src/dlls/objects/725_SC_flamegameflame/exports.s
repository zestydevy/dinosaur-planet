.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCFlameGameFlame_ctor
.dword SCFlameGameFlame_dtor

# export table
/*0*/ .dword SCFlameGameFlame_setup
/*1*/ .dword SCFlameGameFlame_control
/*2*/ .dword SCFlameGameFlame_update
/*3*/ .dword SCFlameGameFlame_print
/*4*/ .dword SCFlameGameFlame_free
/*5*/ .dword SCFlameGameFlame_get_model_flags
/*6*/ .dword SCFlameGameFlame_get_data_size
/*7*/ .dword SCFlameGameFlame_func_354
/*8*/ .dword SCFlameGameFlame_func_3A4
/*9*/ .dword SCFlameGameFlame_func_3BC
/*10*/ .dword SCFlameGameFlame_func_3D4
