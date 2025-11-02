.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword mainmenu_ctor
.dword mainmenu_dtor

# export table
.dword mainmenu_update1
.dword mainmenu_update2
.dword mainmenu_draw
