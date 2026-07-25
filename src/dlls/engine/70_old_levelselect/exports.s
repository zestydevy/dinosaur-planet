.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword old_levelselect_ctor
.dword old_levelselect_dtor

# export table
/*0*/ .dword old_levelselect_update1
/*1*/ .dword old_levelselect_update2
/*2*/ .dword old_levelselect_draw
