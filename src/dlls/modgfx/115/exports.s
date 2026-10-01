.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_115_ctor
.dword dll_115_dtor

# export table
/*0*/ .dword dll_115_modgfx_Func0
