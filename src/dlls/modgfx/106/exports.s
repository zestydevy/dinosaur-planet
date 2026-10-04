.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_106_ctor
.dword dll_106_dtor

# export table
/*0*/ .dword dll_106_modgfx_Spawn
