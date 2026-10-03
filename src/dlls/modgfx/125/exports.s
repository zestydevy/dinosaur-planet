.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_125_ctor
.dword dll_125_dtor

# export table
/*0*/ .dword dll_125_modgfx_Func0
