.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFMagicWall_ctor
.dword CFMagicWall_dtor

# export table
/*0*/ .dword CFMagicWall_setup
/*1*/ .dword CFMagicWall_control
/*2*/ .dword CFMagicWall_update
/*3*/ .dword CFMagicWall_print
/*4*/ .dword CFMagicWall_free
/*5*/ .dword CFMagicWall_get_model_flags
/*6*/ .dword CFMagicWall_get_data_size
