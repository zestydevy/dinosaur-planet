.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHswapstone_ctor
.dword SHswapstone_dtor

# export table
.dword SHswapstone_setup
.dword SHswapstone_control
.dword SHswapstone_update
.dword SHswapstone_print
.dword SHswapstone_free
.dword SHswapstone_get_model_flags
.dword SHswapstone_get_data_size
