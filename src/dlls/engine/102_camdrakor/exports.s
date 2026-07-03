.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camdrakor_ctor
.dword camdrakor_dtor

# export table
/*0*/ .dword camdrakor_func_18
/*1*/ .dword camdrakor_func_138
/*2*/ .dword camdrakor_func_61C
/*3*/ .dword camdrakor_func_65C
