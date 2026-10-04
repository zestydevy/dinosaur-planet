.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_104_ctor
.dword dll_104_dtor

# export table
/*0*/ .dword dll_104_modgfx_Spawn
