.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword flash_ctor
.dword flash_dtor

# export table
.dword flash_load_game
.dword flash_save_game
