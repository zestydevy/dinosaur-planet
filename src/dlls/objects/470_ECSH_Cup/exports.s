.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ECSHCup_ctor
.dword ECSHCup_dtor

# export table
/*0*/ .dword ECSHCup_obj_Setup
/*1*/ .dword ECSHCup_obj_Control
/*2*/ .dword ECSHCup_obj_Update
/*3*/ .dword ECSHCup_obj_Print
/*4*/ .dword ECSHCup_free
/*5*/ .dword ECSHCup_obj_GetModelFlags
/*6*/ .dword ECSHCup_obj_GetDataSize
