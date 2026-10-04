.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_122_ctor
.dword dll_122_dtor

# export table
/*0*/ .dword dll_122_modgfx_Spawn
