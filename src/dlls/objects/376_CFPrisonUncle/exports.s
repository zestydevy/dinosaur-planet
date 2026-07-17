.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFPrisonUncle_ctor
.dword CFPrisonUncle_dtor

# export table
/*0*/ .dword CFPrisonUncle_obj_Setup
/*1*/ .dword CFPrisonUncle_obj_Control
/*2*/ .dword CFPrisonUncle_obj_Update
/*3*/ .dword CFPrisonUncle_obj_Print
/*4*/ .dword CFPrisonUncle_obj_Free
/*5*/ .dword CFPrisonUncle_obj_GetModelFlags
/*6*/ .dword CFPrisonUncle_obj_GetDataSize
