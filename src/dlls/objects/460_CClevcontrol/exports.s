.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CClevcontrol_ctor
.dword CClevcontrol_dtor

# export table
.dword CClevcontrol_setup
.dword CClevcontrol_control
.dword CClevcontrol_update
.dword CClevcontrol_print
.dword CClevcontrol_free
.dword CClevcontrol_get_model_flags
.dword CClevcontrol_get_data_size
