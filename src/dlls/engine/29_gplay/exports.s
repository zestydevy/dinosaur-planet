.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword gplay_ctor
.dword gplay_dtor

# export table
.dword gplay_erase_save
.dword gplay_init_save
.dword gplay_load_save
.dword gplay_copy_save
.dword gplay_func_6AC
.dword gplay_func_94C
.dword gplay_func_958
.dword gplay_start_game
.dword gplay_restart_set
.dword gplay_restart_goto
.dword gplay_restart_clear
.dword gplay_func_D70
.dword gplay_func_8D8
.dword gplay_func_79C
.dword gplay_func_930
.dword gplay_func_143C
.dword gplay_func_139C
.dword gplay_func_1378
.dword gplay_func_14F0
.dword gplay_func_16C4
.dword gplay_func_1590
.dword gplay_func_15B8
.dword gplay_func_163C
.dword gplay_func_1680
.dword gplay_func_1014
.dword gplay_func_109C
.dword gplay_func_10F4
.dword gplay_func_115C
.dword gplay_func_E90
.dword gplay_func_EAC
.dword gplay_func_1254
.dword gplay_func_121C
.dword gplay_func_1238
.dword gplay_func_E74
.dword gplay_func_ED4
.dword gplay_func_F04
.dword gplay_func_F30
.dword gplay_func_F60
.dword gplay_func_FA8
.dword gplay_func_FE8
.dword gplay_func_1974
.dword gplay_func_19B8
.dword gplay_func_1270
.dword gplay_func_19FC
.dword gplay_func_1A48
.dword gplay_func_1A90
.dword gplay_func_1AF8
.dword gplay_func_1B78
.dword gplay_func_1BC4
