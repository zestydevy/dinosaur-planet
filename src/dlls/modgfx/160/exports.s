.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_160_ctor
.dword dll_160_dtor

# export table
/*0*/ .dword dll_160_modgfx_Func0
