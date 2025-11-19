.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DRbullet_ctor
.dword DRbullet_dtor

# export table
/*0*/ .dword DRbullet_setup
/*1*/ .dword DRbullet_control
/*2*/ .dword DRbullet_update
/*3*/ .dword DRbullet_print
/*4*/ .dword DRbullet_free
/*5*/ .dword DRbullet_get_model_flags
/*6*/ .dword DRbullet_get_data_size
/*7*/ .dword DRbullet_check_if_recycleable
/*8*/ .dword DRbullet_recycle
/*9*/ .dword DRbullet_free_if_inactive
/*10*/ .dword DRbullet_set_explode_state
