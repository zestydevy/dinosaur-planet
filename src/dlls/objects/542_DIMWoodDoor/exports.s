.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMWoodDoor_ctor
.dword DIMWoodDoor_dtor

# export table
/*0*/ .dword DIMWoodDoor_obj_Setup
/*1*/ .dword DIMWoodDoor_obj_Control
/*2*/ .dword DIMWoodDoor_obj_Update
/*3*/ .dword DIMWoodDoor_obj_Print
/*4*/ .dword DIMWoodDoor_obj_Free
/*5*/ .dword DIMWoodDoor_obj_GetModelFlags
/*6*/ .dword DIMWoodDoor_obj_GetDataSize
