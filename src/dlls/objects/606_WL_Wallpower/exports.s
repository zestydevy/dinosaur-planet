.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMwallpower_ctor
.dword WMwallpower_dtor

# export table
/*0*/ .dword WMwallpower_setup
/*1*/ .dword WMwallpower_control
/*2*/ .dword WMwallpower_update
/*3*/ .dword WMwallpower_print
/*4*/ .dword WMwallpower_free
/*5*/ .dword WMwallpower_get_model_flags
/*6*/ .dword WMwallpower_get_data_size
