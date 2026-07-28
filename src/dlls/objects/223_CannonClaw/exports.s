.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CannonClaw_ctor
.dword CannonClaw_dtor

# export table
.dword CannonClaw_obj_Setup
.dword CannonClaw_obj_Control
.dword CannonClaw_obj_Update
.dword CannonClaw_obj_Print
.dword CannonClaw_obj_Free
.dword CannonClaw_obj_GetModelFlags
.dword CannonClaw_obj_GetDataSize
