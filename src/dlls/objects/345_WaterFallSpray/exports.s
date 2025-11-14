.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WaterFallSpray_ctor
.dword WaterFallSpray_dtor

# export table
.dword WaterFallSpray_setup
.dword WaterFallSpray_control
.dword WaterFallSpray_update
.dword WaterFallSpray_print
.dword WaterFallSpray_free
.dword WaterFallSpray_get_model_flags
.dword WaterFallSpray_get_data_size
