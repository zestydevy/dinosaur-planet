.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_150_ctor
.dword dll_150_dtor

# export table
/*0*/ .dword dll_150_modgfx_Func0
