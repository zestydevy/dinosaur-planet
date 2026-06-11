.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_70_ctor
.dword dll_70_dtor

# export table
/*0*/ .dword dll_70_update1
/*1*/ .dword dll_70_update2
/*2*/ .dword dll_70_draw
