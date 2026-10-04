.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_123_ctor
.dword dll_123_dtor

# export table
/*0*/ .dword dll_123_modgfx_Spawn
