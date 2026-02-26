.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SpellOverlay_ctor
.dword SpellOverlay_dtor

# export table
/*0*/ .dword SpellOverlay_setup
/*1*/ .dword SpellOverlay_control
/*2*/ .dword SpellOverlay_update
/*3*/ .dword SpellOverlay_print
/*4*/ .dword SpellOverlay_free
/*5*/ .dword SpellOverlay_get_model_flags
/*6*/ .dword SpellOverlay_get_data_size
/*7*/ .dword SpellOverlay_fade_out
