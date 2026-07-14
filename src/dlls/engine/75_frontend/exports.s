.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword frontend_ctor
.dword frontend_dtor

# export table
/*0*/ .dword frontend_create_slider
/*1*/ .dword frontend_create_checkbox
/*2*/ .dword frontend_create_list
/*3*/ .dword frontend_free
/*4*/ .dword frontend_update
/*5*/ .dword frontend_draw
/*6*/ .dword frontend_get_highlight_state
/*7*/ .dword frontend_set_highlight_state
/*8*/ .dword frontend_get_value
/*9*/ .dword frontend_set_value
/*10*/ .dword frontend_did_value_change
/*11*/ .dword frontend_set_unlock_state
