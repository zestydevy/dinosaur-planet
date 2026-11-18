.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword XYZAnimator_ctor
.dword XYZAnimator_dtor

# export table
/*0*/ .dword XYZAnimator_obj_Setup
/*1*/ .dword XYZAnimator_obj_Control
/*2*/ .dword XYZAnimator_obj_Update
/*3*/ .dword XYZAnimator_obj_Print
/*4*/ .dword XYZAnimator_obj_Free
/*5*/ .dword XYZAnimator_obj_GetModelFlags
/*6*/ .dword XYZAnimator_obj_GetDataSize
