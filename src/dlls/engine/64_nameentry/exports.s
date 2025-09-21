.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_64_ctor
.dword dll_64_dtor

# export table
.dword dll_64_update1
.dword dll_64_update2
.dword dll_64_draw
