.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_124_ctor
.dword dll_124_dtor

# export table
/*0*/ .dword dll_124_modgfx_Spawn
