.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHswapstone_ctor
.dword SHswapstone_dtor

# export table
.dword SHswapstone_create
.dword SHswapstone_update
.dword SHswapstone_func_278
.dword SHswapstone_draw
.dword SHswapstone_destroy
.dword SHswapstone_get_model_flags
.dword SHswapstone_get_state_size
