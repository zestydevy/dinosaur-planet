.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_60_ctor
.dword dll_60_dtor

# export table
.dword dll_60_update1
.dword dll_60_update2
.dword dll_60_draw
