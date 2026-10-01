.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sfxPlayer_ctor
.dword sfxPlayer_dtor

# export table
.dword sfxPlayer_obj_Setup
.dword sfxPlayer_obj_Control
.dword sfxPlayer_obj_Update
.dword sfxPlayer_obj_Print
.dword sfxPlayer_obj_Free
.dword sfxPlayer_obj_GetModelFlags
.dword sfxPlayer_obj_GetDataSize
