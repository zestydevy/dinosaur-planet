.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFPrisonGuard_ctor
.dword CFPrisonGuard_dtor

# export table
/*0*/ .dword CFPrisonGuard_obj_Setup
/*1*/ .dword CFPrisonGuard_obj_Control
/*2*/ .dword CFPrisonGuard_obj_Update
/*3*/ .dword CFPrisonGuard_obj_Print
/*4*/ .dword CFPrisonGuard_obj_Free
/*5*/ .dword CFPrisonGuard_obj_GetModelFlags
/*6*/ .dword CFPrisonGuard_obj_GetDataSize
