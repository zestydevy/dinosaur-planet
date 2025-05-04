.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_63_ctor
.dword dll_63_dtor

# export table
.dword dll_63_update1
.dword dll_63_update2
.dword dll_63_draw
