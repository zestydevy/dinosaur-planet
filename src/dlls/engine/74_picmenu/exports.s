.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword picmenu_ctor
.dword picmenu_dtor

# export table
.dword picmenu_set_items
.dword picmenu_clear_items
.dword picmenu_update
.dword picmenu_draw
.dword picmenu_get_selected_item
.dword picmenu_set_selected_item
.dword picmenu_get_item_override
.dword picmenu_set_item_override
.dword picmenu_update_text
.dword picmenu_get_highlight_alpha
.dword picmenu_update_flags
.dword picmenu_set_opacity
.dword picmenu_redraw_all
