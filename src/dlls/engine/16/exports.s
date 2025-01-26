.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword init
.dword fini

# export table
.dword func_0000
.dword func_0001
.dword func_0002
