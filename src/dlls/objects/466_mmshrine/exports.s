.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MMshrine_ctor
.dword MMshrine_dtor

# export table
.dword dll_466_func_18
.dword dll_466_func_1BC
.dword MMshrine_func_B3C
.dword MMshrine_draw
.dword MMshrine_destroy
.dword MMshrine_get_model_flags
.dword MMshrine_get_state_size
