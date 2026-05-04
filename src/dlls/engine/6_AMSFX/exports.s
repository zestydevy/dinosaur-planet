.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword amsfx_ctor
.dword amsfx_dtor

# export table
.dword amsfx_func_338
.dword amsfx_func_480
.dword amsfx_play
.dword amsfx_func_7E4
.dword amsfx_set_vol
.dword amsfx_set_pitch
.dword amsfx_stop
.dword amsfx_stop_object
.dword amsfx_is_playing
.dword amsfx_find_emitters_in_range
.dword amsfx_func_DCC
.dword amsfx_get_default
.dword amsfx_flush_cache
.dword amsfx_snd_play_ex
.dword amsfx_play2
.dword amsfx_play3
.dword amsfx_free_object
.dword amsfx_update_doppler
.dword amsfx_water_falls_control
.dword amsfx_water_falls_set_flags
.dword amsfx_play_custom
