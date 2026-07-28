.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MoonSeedReceptacle_ctor
.dword MoonSeedReceptacle_dtor

# export table
/*0*/ .dword MoonSeedReceptacle_obj_Setup
/*1*/ .dword MoonSeedReceptacle_obj_Control
/*2*/ .dword MoonSeedReceptacle_obj_Update
/*3*/ .dword MoonSeedReceptacle_obj_Print
/*4*/ .dword MoonSeedReceptacle_obj_Free
/*5*/ .dword MoonSeedReceptacle_obj_GetModelFlags
/*6*/ .dword MoonSeedReceptacle_obj_GetDataSize
/*7*/ .dword MoonSeedReceptacle_Func_BEC
/*8*/ .dword MoonSeedReceptacle_Func_D00
/*9*/ .dword MoonSeedReceptacle_Func_D18
/*10*/ .dword MoonSeedReceptacle_Func_D30
