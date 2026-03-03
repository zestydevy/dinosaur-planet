.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CamControl_ctor
.dword CamControl_dtor

# export table
/*0*/ .dword CamControl_init_data
/*1*/ .dword CamControl_tick
/*2*/ .dword CamControl_get_data
/*3*/ .dword CamControl_get_dll_ID
/*4*/ .dword CamControl_get_active_module
/*5*/ .dword CamControl_get_camnormal_module
/*6*/ .dword CamControl_change_camera_module
/*7*/ .dword CamControl_get_camera_action
/*8*/ .dword CamControl_change_mode
/*9*/ .dword CamControl_store_player
/*10*/ .dword CamControl_reposition_player
/*11*/ .dword CamControl_move_camera_by_delta
/*12*/ .dword CamControl_func_BD4
/*13*/ .dword CamControl_get_player_to_camera_distances
/*14*/ .dword CamControl_get_target_object
/*15*/ .dword CamControl_get_highlighted_object
/*16*/ .dword CamControl_set_target_flag_2
/*17*/ .dword CamControl_set_target_object
/*18*/ .dword CamControl_has_interpolation_finished
/*19*/ .dword CamControl_lock_icon_tick
/*20*/ .dword CamControl_lock_icon_print
/*21*/ .dword CamControl_set_letterbox_goal
/*22*/ .dword CamControl_func_7B4
/*23*/ .dword CamControl_get_target_gametextID
/*24*/ .dword CamControl_apply_highlight_flags
/*25*/ .dword CamControl_apply_target_flags
/*26*/ .dword CamControl_revert_camera_module
