.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword cmdmenu_ctor
.dword cmdmenu_dtor

# export table
.dword cmdmenu_func_308
.dword cmdmenu_func_35C
.dword cmdmenu_print
.dword cmdmenu_func_1290
.dword cmdmenu_func_12EC
.dword cmdmenu_get_target_objects
.dword cmdmenu_func_DC4
.dword cmdmenu_was_item_used
.dword cmdmenu_func_E2C
.dword cmdmenu_func_F24
.dword cmdmenu_func_F40
.dword cmdmenu_func_70A0
.dword cmdmenu_func_130C
.dword cmdmenu_func_1338
.dword cmdmenu_func_13F4
.dword cmdmenu_disable_buttons
.dword cmdmenu_func_6984
.dword cmdmenu_func_69F8
.dword cmdmenu_func_6B00
.dword cmdmenu_toggle_forced_stats_display
.dword cmdmenu_energy_bar_create
.dword cmdmenu_energy_bar_set
.dword cmdmenu_energy_bar_free
.dword cmdmenu_energy_bar_fadeout
