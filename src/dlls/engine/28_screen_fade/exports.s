.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword screen_fade_ctor
.dword screen_fade_dtor

# export table
.dword screen_fade_draw
.dword screen_fade_fade
.dword screen_fade_fade_reversed
.dword screen_fade_func_3AC
.dword screen_fade_is_complete
.dword screen_fade_get_progress
