.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword old_mainmenu_ctor
.dword old_mainmenu_dtor

# export table
/*0*/ .dword old_mainmenu_func_C4
/*1*/ .dword old_mainmenu_func_D0
/*2*/ .dword old_mainmenu_func_D8
