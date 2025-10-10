.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFMagicWall_ctor
.dword CFMagicWall_dtor

# export table
/*0*/ .dword CFMagicWall_create
/*1*/ .dword CFMagicWall_update
/*2*/ .dword CFMagicWall_func_2C0
/*3*/ .dword CFMagicWall_draw
/*4*/ .dword CFMagicWall_destroy
/*5*/ .dword CFMagicWall_get_model_flags
/*6*/ .dword CFMagicWall_get_state_size
