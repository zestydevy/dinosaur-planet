.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ctor
.dword dtor

# export table
.dword func_18
.dword func_A4
