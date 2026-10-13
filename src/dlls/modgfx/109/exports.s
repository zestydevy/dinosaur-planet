.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_109_ctor
.dword dll_109_dtor

# export table
/*0*/ .dword dll_109_modgfx_Spawn
