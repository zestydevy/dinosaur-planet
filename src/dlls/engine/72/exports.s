.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_72_ctor
.dword dll_72_dtor

# export table
/*0*/ .dword dll_72_update1
/*1*/ .dword dll_72_update2
/*2*/ .dword dll_72_draw
