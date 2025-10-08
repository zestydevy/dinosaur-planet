.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword levelname_ctor
.dword levelname_dtor

# export table
.dword levelname_create
.dword levelname_update
.dword levelname_func_304
.dword levelname_draw
.dword levelname_destroy
.dword levelname_func_4E4
.dword levelname_get_state_size
