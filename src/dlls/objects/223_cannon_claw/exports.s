.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword cannon_claw_ctor
.dword cannon_claw_dtor

# export table
.dword cannon_claw_setup
.dword cannon_claw_control
.dword cannon_claw_update
.dword cannon_claw_print
.dword cannon_claw_free
.dword cannon_claw_get_model_flags
.dword cannon_claw_get_data_size
