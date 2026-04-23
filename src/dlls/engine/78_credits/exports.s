.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword credits_ctor
.dword credits_dtor

# export table
/*0*/ .dword credits_update1
/*1*/ .dword credits_update2
/*2*/ .dword credits_draw
