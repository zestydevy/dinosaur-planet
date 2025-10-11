.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_296_ctor
.dword dll_296_dtor

# export table
.dword dll_295_setup
.dword dll_295_control
.dword dll_295_update
.dword dll_295_print
.dword dll_295_free
.dword dll_296_get_model_flags
.dword dll_296_get_data_size
