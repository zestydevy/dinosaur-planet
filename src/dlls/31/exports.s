.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_31_ctor
.dword dll_31_dtor

# export table
.dword dll_31_flashLoadGame
.dword dll_31_flashSaveGame
