.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWtreebridge_ctor
.dword NWtreebridge_dtor

# export table
/*0*/ .dword NWtreebridge_setup
/*1*/ .dword NWtreebridge_control
/*2*/ .dword NWtreebridge_update
/*3*/ .dword NWtreebridge_print
/*4*/ .dword NWtreebridge_free
/*5*/ .dword NWtreebridge_get_model_flags
/*6*/ .dword NWtreebridge_get_data_size
