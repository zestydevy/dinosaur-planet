.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_112_ctor
.dword dll_112_dtor

# export table
/*0*/ .dword dll_112_modgfx_Spawn
