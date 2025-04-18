.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword screens_ctor
.dword screens_dtor

# export table
.dword screens_show_screen
.dword screens_hide_screen
.dword screens_draw
