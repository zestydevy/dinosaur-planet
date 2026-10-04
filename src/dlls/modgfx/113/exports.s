.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_113_ctor
.dword dll_113_dtor

# export table
/*0*/ .dword dll_113_modgfx_Spawn
