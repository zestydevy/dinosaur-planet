.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_SpellPlace_ctor
.dword VFP_SpellPlace_dtor

# export table
.dword VFP_SpellPlace_create
.dword VFP_SpellPlace_update
.dword VFP_SpellPlace_func_1AC
.dword VFP_SpellPlace_draw
.dword VFP_SpellPlace_destroy
.dword VFP_SpellPlace_get_model_flags
.dword VFP_SpellPlace_get_state_size
