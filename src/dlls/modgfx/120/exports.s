.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_120_ctor
.dword dll_120_dtor

# export table
/*0*/ .dword dll_120_modgfx_Spawn
