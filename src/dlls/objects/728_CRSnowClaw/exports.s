.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CRSnowClaw_ctor
.dword CRSnowClaw_dtor

# export table
/*0*/ .dword CRSnowClaw_obj_Setup
/*1*/ .dword CRSnowClaw_obj_Control
/*2*/ .dword CRSnowClaw_obj_Update
/*3*/ .dword CRSnowClaw_obj_Print
/*4*/ .dword CRSnowClaw_obj_Free
/*5*/ .dword CRSnowClaw_obj_GetModelFlags
/*6*/ .dword CRSnowClaw_obj_GetDataSize
