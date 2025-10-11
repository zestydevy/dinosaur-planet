.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sfxplayer_ctor
.dword sfxplayer_dtor

# export table
.dword sfxplayer_setup
.dword sfxplayer_control
.dword sfxplayer_update
.dword sfxplayer_print
.dword sfxplayer_free
.dword sfxplayer_get_model_flags
.dword sfxplayer_get_data_size
