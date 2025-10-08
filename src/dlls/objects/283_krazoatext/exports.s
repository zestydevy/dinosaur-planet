.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword krazoatext_ctor
.dword krazoatext_dtor

# export table
.dword krazoatext_create
.dword krazoatext_update
.dword krazoatext_func_324
.dword krazoatext_draw
.dword krazoatext_destroy
.dword krazoatext_func_414
.dword krazoatext_get_state_size
.dword krazoatext_print
