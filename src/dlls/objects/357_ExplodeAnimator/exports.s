.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ExplodeAnimator_ctor
.dword ExplodeAnimator_dtor

# export table
/*0*/ .dword ExplodeAnimator_obj_Setup
/*1*/ .dword ExplodeAnimator_obj_Control
/*2*/ .dword ExplodeAnimator_obj_Update
/*3*/ .dword ExplodeAnimator_obj_Print
/*4*/ .dword ExplodeAnimator_obj_Free
/*5*/ .dword ExplodeAnimator_obj_GetModelFlags
/*6*/ .dword ExplodeAnimator_obj_GetDataSize
