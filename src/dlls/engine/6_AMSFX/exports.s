.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword amSfx_ctor
.dword amSfx_dtor

# export table
.dword amSfx_Func338
.dword amSfx_Func480
.dword amSfx_Play
.dword amSfx_Func7E4
.dword amSfx_SetVol
.dword amSfx_SetPitch
.dword amSfx_Stop
.dword amSfx_StopObject
.dword amSfx_IsPlaying
.dword amSfx_FindEmittersInRange
.dword amSfx_FuncDCC
.dword amSfx_GetDefault
.dword amSfx_FlushCache
.dword amSfx_SndPlayEx
.dword amSfx_Play2
.dword amSfx_Play3
.dword amSfx_FreeObject
.dword amSfx_UpdateDoppler
.dword amSfx_WaterFallsControl
.dword amSfx_WaterFallsSetFlags
.dword amSfx_PlayCustom
