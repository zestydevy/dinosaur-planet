.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camtalk1_ctor
.dword camtalk1_dtor

# export table
.dword camtalk1_setup
.dword camtalk1_control
.dword camtalk1_free
.dword camtalk1_func_7C
