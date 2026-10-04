.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_121_ctor
.dword dll_121_dtor

# export table
/*0*/ .dword dll_121_modgfx_Spawn
