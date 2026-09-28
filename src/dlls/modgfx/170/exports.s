.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_170_ctor
.dword dll_170_dtor

# export table
/*0*/ .dword dll_170_modgfx_Func0
