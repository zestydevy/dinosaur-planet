.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword animator_ctor
.dword animator_dtor

# export table
.dword animator_create
.dword animator_update
.dword animator_func_300
.dword animator_draw
.dword animator_destroy
.dword animator_func_36C
.dword animator_get_state_size
