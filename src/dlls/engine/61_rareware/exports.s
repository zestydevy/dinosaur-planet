.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword rareware_ctor
.dword rareware_dtor

# export table
.dword rareware_update1
.dword rareware_update2
.dword rareware_draw
