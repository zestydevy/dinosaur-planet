.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_128_ctor
.dword dll_128_dtor

# export table
/*0*/ .dword dll_128_modgfx_Func0
