.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_SpiritSet_ctor
.dword WL_SpiritSet_dtor

# export table
.dword WL_SpiritSet_setup
.dword WL_SpiritSet_control
.dword WL_SpiritSet_update
.dword WL_SpiritSet_print
.dword WL_SpiritSet_free
.dword WL_SpiritSet_get_model_flags
.dword WL_SpiritSet_get_data_size
