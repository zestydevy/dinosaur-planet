.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCspellpage_ctor
.dword CCspellpage_dtor

# export table
.dword CCspellpage_setup
.dword CCspellpage_control
.dword CCspellpage_update
.dword CCspellpage_print
.dword CCspellpage_free
.dword CCspellpage_get_model_flags
.dword CCspellpage_get_data_size
