.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Tumbleweed_ctor
.dword Tumbleweed_dtor

# export table
.dword Tumbleweed_setup
.dword Tumbleweed_control
.dword Tumbleweed_update
.dword Tumbleweed_print
.dword Tumbleweed_free
.dword Tumbleweed_get_model_flags
.dword Tumbleweed_get_data_size
.dword Tumbleweed_get_state
.dword Tumbleweed_set_home
.dword Tumbleweed_fall
.dword Tumbleweed_gravitate_towards_point
.dword Tumbleweed_is_gravitating
.dword Tumbleweed_store_player
