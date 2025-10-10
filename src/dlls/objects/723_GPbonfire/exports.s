.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GPbonfire_ctor
.dword GPbonfire_dtor

# export table
.dword GPbonfire_create
.dword GPbonfire_update
.dword GPbonfire_func_7F4
.dword GPbonfire_draw
.dword GPbonfire_destroy
.dword GPbonfire_get_model_flags
.dword GPbonfire_get_state_size
.dword GPbonfire_func_910
.dword GPbonfire_func_A08
.dword GPbonfire_func_A20
.dword GPbonfire_func_A38
