.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_110_ctor
.dword dll_110_dtor

# export table
/*0*/ .dword dll_110_modgfx_Func0
