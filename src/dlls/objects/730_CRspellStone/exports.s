.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CRSpellStone_ctor
.dword CRSpellStone_dtor

# export table
/*0*/ .dword CRSpellStone_setup
/*1*/ .dword CRSpellStone_control
/*2*/ .dword CRSpellStone_update
/*3*/ .dword CRSpellStone_print
/*4*/ .dword CRSpellStone_free
/*5*/ .dword CRSpellStone_get_model_flags
/*6*/ .dword CRSpellStone_get_data_size
/*7*/ .dword CRSpellStone_set_state
/*8*/ .dword CRSpellStone_is_not_rescued
