.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camseq_ctor
.dword camseq_dtor

# export table
.dword camseq_setup
.dword camseq_control
.dword camseq_free
.dword camseq_func_7C
