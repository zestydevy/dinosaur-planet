.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword n_pausemenu_ctor
.dword n_pausemenu_dtor

# export table
.dword n_pausemenu_update
.dword n_pausemenu_func_524
.dword n_pausemenu_draw
