.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMExplosion_ctor
.dword DIMExplosion_dtor

# export table
/*0*/ .dword DIMExplosion_obj_Setup
/*1*/ .dword DIMExplosion_obj_Control
/*2*/ .dword DIMExplosion_obj_Update
/*3*/ .dword DIMExplosion_obj_Print
/*4*/ .dword DIMExplosion_obj_Free
/*5*/ .dword DIMExplosion_obj_GetModelFlags
/*6*/ .dword DIMExplosion_obj_GetDataSize
