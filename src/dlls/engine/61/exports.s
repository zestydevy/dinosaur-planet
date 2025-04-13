.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_61_ctor
.dword dll_61_dtor

# export table
.dword dll_61_update1
.dword dll_61_update2
.dword dll_61_draw
