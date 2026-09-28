.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_183_ctor
.dword dll_183_dtor

# export table
/*0*/ .dword dll_183_modgfx_Func0
