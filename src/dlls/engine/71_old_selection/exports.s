.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_71_ctor
.dword dll_71_dtor

# export table
/*0*/ .dword dll_71_update1
/*1*/ .dword dll_71_update2
/*2*/ .dword dll_71_draw
