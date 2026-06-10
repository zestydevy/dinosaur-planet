.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_67_ctor
.dword dll_67_dtor

# export table
/*0*/ .dword dll_67_func_200
/*1*/ .dword dll_67_func_868
/*2*/ .dword dll_67_func_870
