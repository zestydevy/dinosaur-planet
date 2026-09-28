.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_130_ctor
.dword dll_130_dtor

# export table
/*0*/ .dword dll_130_modgfx_Func0
