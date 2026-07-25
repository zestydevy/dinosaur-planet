.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_80_ctor
.dword dll_80_dtor

# export table
/*0*/ .dword dll_80_update1
/*1*/ .dword dll_80_update2
/*2*/ .dword dll_80_draw
