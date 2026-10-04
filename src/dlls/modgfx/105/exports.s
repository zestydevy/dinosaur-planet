.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_105_ctor
.dword dll_105_dtor

# export table
/*0*/ .dword dll_105_modgfx_Spawn
