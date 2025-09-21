.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword mainmenu_ctor
.dword mainmenu_dtor

# export table
.dword mainmenu_update
.dword mainmenu_func_764
.dword mainmenu_draw
