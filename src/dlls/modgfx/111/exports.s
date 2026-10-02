.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_111_ctor
.dword dll_111_dtor

# export table
/*0*/ .dword dll_111_modgfx_Func0
