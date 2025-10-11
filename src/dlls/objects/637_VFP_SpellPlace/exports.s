.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_SpellPlace_ctor
.dword VFP_SpellPlace_dtor

# export table
.dword VFP_SpellPlace_setup
.dword VFP_SpellPlace_control
.dword VFP_SpellPlace_update
.dword VFP_SpellPlace_print
.dword VFP_SpellPlace_free
.dword VFP_SpellPlace_get_model_flags
.dword VFP_SpellPlace_get_state_size
