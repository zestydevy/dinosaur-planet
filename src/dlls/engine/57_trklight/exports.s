.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword trklight_ctor
.dword trklight_dtor

# export table
/*0*/ .dword trklight_Init
/*1*/ .dword trklight_DoAction
/*2*/ .dword trklight_GetLights
/*3*/ .dword trklight_Tick
