.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMWizard_ctor
.dword WMWizard_dtor

# export table
/*0*/ .dword WMWizard_setup
/*1*/ .dword WMWizard_control
/*2*/ .dword WMWizard_update
/*3*/ .dword WMWizard_print
/*4*/ .dword WMWizard_free
/*5*/ .dword WMWizard_get_model_flags
/*6*/ .dword WMWizard_get_data_size
