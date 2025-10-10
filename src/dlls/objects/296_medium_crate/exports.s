.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_296_ctor
.dword dll_296_dtor

# export table
.dword dll_296_create
.dword dll_296_update
.dword dll_296_func_9C0
.dword dll_296_draw
.dword dll_296_destroy
.dword dll_296_get_model_flags
.dword dll_296_get_state_size
