.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword init
.dword fini

# export table
.dword dll_365_create
.dword dll_365_update
.dword func_0002
.dword dll_365_draw
.dword dll_365_destroy
.dword func_0005
.dword dll_365_get_state_size
