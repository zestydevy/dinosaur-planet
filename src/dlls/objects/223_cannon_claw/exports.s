.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword cannon_claw_ctor
.dword cannon_claw_dtor

# export table
.dword cannon_claw_create
.dword cannon_claw_update
.dword cannon_claw_func_200
.dword cannon_claw_draw
.dword cannon_claw_destroy
.dword cannon_claw_func_27C
.dword cannon_claw_get_state_size
