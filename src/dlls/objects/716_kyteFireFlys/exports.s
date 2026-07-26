.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KyteFireFlys_ctor
.dword KyteFireFlys_dtor

# export table
/*0*/ .dword KyteFireFlys_obj_Setup
/*1*/ .dword KyteFireFlys_obj_Control
/*2*/ .dword KyteFireFlys_obj_Update
/*3*/ .dword KyteFireFlys_obj_Print
/*4*/ .dword KyteFireFlys_obj_Free
/*5*/ .dword KyteFireFlys_obj_GetModelFlags
/*6*/ .dword KyteFireFlys_obj_GetDataSize
/*7*/ .dword KyteFireFlys_KyteTarget_Interact
/*8*/ .dword KyteFireFlys_KyteTarget_Func_49C
/*9*/ .dword KyteFireFlys_KyteTarget_Approach
/*10*/ .dword KyteFireFlys_KyteTarget_Func_4CC
