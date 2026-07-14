.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_69_ctor
.dword dll_69_dtor

# export table
/*0*/ .dword dll_69_update1
/*1*/ .dword dll_69_update2
/*2*/ .dword dll_69_draw
