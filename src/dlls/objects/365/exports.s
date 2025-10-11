.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword init
.dword fini

# export table
.dword dll_365_setup
.dword dll_365_control
.dword dll_365_update
.dword dll_365_print
.dword dll_365_free
.dword dll_365_get_model_flags
.dword dll_365_get_data_size
