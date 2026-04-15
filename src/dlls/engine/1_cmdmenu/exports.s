.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword cmdmenu_ctor
.dword cmdmenu_dtor

# export table
.dword cmdmenu_control
.dword cmdmenu_func_35C
.dword cmdmenu_print
.dword cmdmenu_pages_clear_used_item_index
.dword cmdmenu_request_new_player_stats_snapshot
.dword cmdmenu_get_target_objects
.dword cmdmenu_was_any_item_used
.dword cmdmenu_was_this_item_used
.dword cmdmenu_was_used_item_in_gamebit_array
.dword cmdmenu_get_page_category
.dword cmdmenu_func_F40
.dword cmdmenu_func_70A0
.dword cmdmenu_func_130C
.dword cmdmenu_func_1338
.dword cmdmenu_func_13F4
.dword cmdmenu_disable_buttons
.dword cmdmenu_set_buttons_override
.dword cmdmenu_info_show
.dword cmdmenu_info_show_tex
.dword cmdmenu_toggle_forced_stats_display
.dword cmdmenu_energy_bar_create
.dword cmdmenu_energy_bar_set
.dword cmdmenu_energy_bar_free
.dword cmdmenu_energy_bar_fadeout
