.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword PortalSpellDoor_ctor
.dword PortalSpellDoor_dtor

# export table
/*0*/ .dword PortalSpellDoor_setup
/*1*/ .dword PortalSpellDoor_control
/*2*/ .dword PortalSpellDoor_update
/*3*/ .dword PortalSpellDoor_print
/*4*/ .dword PortalSpellDoor_free
/*5*/ .dword PortalSpellDoor_get_model_flags
/*6*/ .dword PortalSpellDoor_get_data_size
