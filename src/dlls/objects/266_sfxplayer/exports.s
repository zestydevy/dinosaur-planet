.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sfxplayer_ctor
.dword sfxplayer_dtor

# export table
.dword sfxplayer_create
.dword sfxplayer_update
.dword sfxplayer_func_458
.dword sfxplayer_draw
.dword sfxplayer_destroy
.dword sfxplayer_func_51C
.dword sfxplayer_get_state_size
