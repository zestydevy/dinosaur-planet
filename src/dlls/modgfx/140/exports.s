.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_140_ctor
.dword dll_140_dtor

# export table
/*0*/ .dword dll_140_modgfx_Func0
