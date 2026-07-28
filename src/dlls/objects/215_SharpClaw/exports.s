.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SharpClaw_ctor
.dword SharpClaw_dtor

# export table
/*0*/ .dword SharpClaw_obj_Setup
/*1*/ .dword SharpClaw_obj_Control
/*2*/ .dword SharpClaw_obj_Update
/*3*/ .dword SharpClaw_obj_Print
/*4*/ .dword SharpClaw_obj_Free
/*5*/ .dword SharpClaw_obj_GetModelFlags
/*6*/ .dword SharpClaw_obj_GetDataSize
/*7*/ .dword SharpClaw_GetLogicState
/*8*/ .dword SharpClaw_ReceiveMessage
