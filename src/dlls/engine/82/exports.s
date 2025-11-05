.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_82_ctor
.dword dll_82_dtor

# export table
/*0*/ .dword dll_82_func_18
