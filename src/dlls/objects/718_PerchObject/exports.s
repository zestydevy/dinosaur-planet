.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword perchobject_ctor
.dword perchobject_dtor

# export table
/*0*/ .dword perchobject_obj_Setup
/*1*/ .dword perchobject_obj_Control
/*2*/ .dword perchobject_obj_Update
/*3*/ .dword perchobject_print
/*4*/ .dword perchobject_obj_Free
/*5*/ .dword perchobject_obj_GetModelFlags
/*6*/ .dword perchobject_obj_GetDataSize
/*7*/ .dword perchobject_KyteTarget_Interact
/*8*/ .dword perchobject_KyteTarget_func_388
/*9*/ .dword perchobject_KyteTarget_Approach
/*10*/ .dword perchobject_KyteTarget_Func_460
