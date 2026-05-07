.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_77_ctor
.dword dll_77_dtor

# export table
.dword dll_77_update1
.dword dll_77_update2
.dword dll_77_draw
