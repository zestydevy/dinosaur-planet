.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_25_ctor
.dword dll_25_dtor

# export table
.dword dll_25_LoadWalkGroups
.dword dll_25_FindWalkGroup
.dword dll_25_IsPointWithinAnyGroup
.dword dll_25_GetPatchGroupContainingPoint
.dword dll_25_IsPointWithinPatchGroup
.dword dll_25_FindNearestEnabledTrickyCurve
.dword dll_25_FindNearestTrickyCurve
.dword dll_25_GetClosestExitOfPatch
.dword dll_25_func_1D30
.dword dll_25_func_21F4
.dword dll_25_func_27A8
.dword dll_25_func_29FC
.dword dll_25_func_2A50
.dword dll_25_func_2BC4
.dword dll_25_func_2CF8
