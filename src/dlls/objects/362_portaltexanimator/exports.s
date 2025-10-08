.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword portaltexanimator_ctor
.dword portaltexanimator_dtor

# export table
.dword portaltexanimator_create
.dword portaltexanimator_update
.dword portaltexanimator_func_414
.dword portaltexanimator_draw
.dword portaltexanimator_destroy
.dword portaltexanimator_func_4C4
.dword portaltexanimator_get_state_size
