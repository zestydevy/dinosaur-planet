.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GuardClaw_ctor
.dword GuardClaw_dtor

# export table
/*0*/ .dword GuardClaw_setup
/*1*/ .dword GuardClaw_control
/*2*/ .dword GuardClaw_update
/*3*/ .dword GuardClaw_print
/*4*/ .dword GuardClaw_free
/*5*/ .dword GuardClaw_get_model_flags
/*6*/ .dword GuardClaw_get_data_size
/*7*/ .dword GuardClaw_GetFSAState
/*8*/ .dword GuardClaw_SendMessage
/*9*/ .dword GuardClaw_SetItem
/*10*/ .dword GuardClaw_Func_884
