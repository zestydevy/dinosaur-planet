.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WaterfallControl_ctor
.dword WaterfallControl_dtor

# export table
/*0*/ .dword WaterfallControl_setup
/*1*/ .dword WaterfallControl_control
/*2*/ .dword WaterfallControl_update
/*3*/ .dword WaterfallControl_print
/*4*/ .dword WaterfallControl_free
/*5*/ .dword WaterfallControl_get_model_flags
/*6*/ .dword WaterfallControl_get_data_size
