.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BalloonBaddie_ctor
.dword BalloonBaddie_dtor

# export table
/*0*/ .dword BalloonBaddie_setup
/*1*/ .dword BalloonBaddie_control
/*2*/ .dword BalloonBaddie_update
/*3*/ .dword BalloonBaddie_print
/*4*/ .dword BalloonBaddie_free
/*5*/ .dword BalloonBaddie_get_model_flags
/*6*/ .dword BalloonBaddie_get_data_size
