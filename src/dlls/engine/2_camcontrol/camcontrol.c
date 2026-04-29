#include "common.h"
#include "game/objects/object.h"
#include "game/objects/interaction_arrow.h"
#include "sys/dll.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "sys/voxmap.h"
#include "dlls/engine/2_camcontrol.h"
#include "unktypes.h"
#include "prevent_bss_reordering.h"

/** Related to gametext that appears when holding R? */
static s16 _data_0[] = {
    0xffff, 0x0000, 0x00f9, 0x00fc, 0x0102, 0x0104, 0x0106, 0x0108, 0x00fa, 0x00fd, 0x0103, 0x0105, 0x0107, 0x0109, 0xff00, 0x0000
};

typedef struct {
    s32 unk0;
    s8 unk4;
} Unk_DLL2_Func888; //Used in similar situations as CameraAction, but has different shorter structure (CameraFunc15Unk_unk74 seems related to all this, too!)

typedef enum {
    LockIcon_STATE_Hidden = 0,
    LockIcon_STATE_Vanish = 1,
    LockIcon_STATE_Highlight_Start = 2,
    LockIcon_STATE_Highlighted = 3,
    LockIcon_STATE_Lock_On = 4
} LockIcon_States;

/*0x0*/ static CamControl_Data sCamDataStruct;
/*0x11C*/ static CamControl_Data* sCamData;
/*0x120*/ static CamControl_Module* sCamModules[20]; //Camera module DLLs (camtalk, camclimb, cam1stperson, etc.)
/*0x170*/ static u8 sCamModuleCount;     //Number of slots currently occupied in the `sCamModules` array
/*0x174*/ static CamControl_Module* sActiveModule; //Active module: the camera module DLL currently in use
/*0x178*/ static s32 sActiveID;          //Active module: DLL ID
/*0x17C*/ static s32 sActiveLoadedIndex; //Active module: The loaded index of the camera module DLL currently in use
/*0x180*/ static s32 sNextID;            //Ease (End Module): DLL ID of next module (when switching camera DLLs)
/*0x184*/ static s32 sActiveFree;        //Active module: free setting
/*0x188*/ static s32 sActiveSetupVal;    //Active Module: arg1 for DLL's setup function (TO-DO: figure this out)
/*0x18C*/ static UNK_PTR* sCamAction;    //Often a CameraAction*, but sometimes points to a shorter 8-byte struct? (TO-DO: figure this out)
/*0x190*/ static u8 sEaseSetupNeeded;    //Boolean, starts a new camera ease
/*0x191*/ static s8 sNextSetupVal;       //Ease (End Module): arg1 for next module's setup function
/*0x192*/ static s8 sNextFree;           //Ease (End Module): free setting for next module
/*0x194*/ static s32 sEaseDuration;      //Duration of easing between camera modules, in frames
/*0x198*/ static u8 sEaseFlags;          //Which components to lerp during ease (x/y/z/yaw/pitch/roll)
/*0x19C*/ static s32 sPreviousID;        //Ease (Start Module): DLL ID of previous module
/*0x1A0*/ static s32 sPreviousFree;      //Ease (Start Module): free setting for previous module
/*0x1A4*/ static s32 sPreviousSetupVal;  //Ease (Start Module): arg1 for previous module's setup function
/*0x1A8*/ static f32 sPlayerX;
/*0x1AC*/ static f32 sPlayerY;
/*0x1B0*/ static f32 sPlayerZ;
/*0x1B4*/ static f32 sPlayerGlobalPositionX;
/*0x1B8*/ static f32 sPlayerGlobalPositionY;
/*0x1BC*/ static f32 sPlayerGlobalPositionZ;
/*0x1C0*/ static f32 sFov;
/*0x1C4*/ static s32 _bss_1C4;          //Set to -1 in ctor/dctor, but otherwise unused?
/*0x1C8*/ static s8 _bss_1C8;           //Set to -1 in ctor/dctor, but otherwise unused?
/*0x1C9*/ static s8 sIconRapidTimer;    //LockIcon spins rapidly for this long at start of lock-on, before decelerating to lower speed
/*0x1CA*/ static s8 sIconState;         //State Machine value for OBJ_LockIcon
/*0x1CC*/ static s16 sIconRotateSpeed;  //Rotation of lock-on icon
/*0x1D0*/ static Object* sLockIcon;     //OBJ_LockIcon
/*0x1D4*/ static s16 sLetterboxHeight;  //Controls the letterboxing at the top/bottom of frame

CamControl_Module* CamControl_get_camnormal_module(void);
void CamControl_change_camera_module(s32 dllID, s32 arg1, s32 arg2, s32 size, void* arg4, s32 easeDuration, u8 easeFlags);
CameraAction* CamControl_get_camera_action(s32 index);
static s32 CamControl_get_loaded_module_index(u16 arg0);
static void CamControl_free_module(s32 arg0);
static s32 CamControl_load_module(u16 arg0, s32 arg1);
static void CamControl_update_camera(CamControl_Data* arg0);
static void CamControl_replace_active_module(u16 camDLLID, CameraAction* camAction);
static void CamControl_store_player_coords(Object* arg0);
static void CamControl_restore_player_coords(Object* obj);
static void CamControl_average_player_speed(CamControl_Data* arg0, Object* arg1);
static Object* CamControl_find_highlight_object(CamControl_Data* arg0, Object* arg1);
static void CamControl_create_LockIcon(void);
static void CamControl_print(Object* arg0, s32 arg1, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);

// offset: 0x0 | ctor
void CamControl_ctor(void* dll) {
    sCamData = &sCamDataStruct;
    bzero(sCamData, sizeof(CamControl_Data));
    
    voxmap_init();

    sActiveID = DLL_NONE;
    sActiveLoadedIndex = DLL_NONE;
    sNextID = DLL_NONE;
    _bss_1C4 = 0;
    _bss_1C8 = -1;
    
    CamControl_create_LockIcon();
}

// offset: 0xA0 | dtor
void CamControl_dtor(void* dll) {
    voxmap_free();
    _bss_1C8 = -1;
}

// offset: 0xE4 | func: 0 | export: 0
void CamControl_init_data(Object* player, f32 initialX, f32 initialY, f32 initialZ) {
    bzero(sCamData, sizeof(CamControl_Data));
    sCamData->srt.transl.x = initialX;
    sCamData->srt.transl.y = initialY;
    sCamData->srt.transl.z = initialZ;
    sCamData->initialCoords.x = initialX;
    sCamData->initialCoords.y = initialY;
    sCamData->initialCoords.z = initialZ;
    sCamData->positionMirror.x = initialX;
    sCamData->positionMirror.y = initialY;
    sCamData->positionMirror.z = initialZ;
    sCamData->player = player;
}

// offset: 0x1A4 | func: 1 | export: 1
void CamControl_tick(void) { //TO-DO: does this really not take updateRate as an argument?
    Object* player;
    f32 tSpeed;
    u8 onTitleScreen;

    if (menu_get_current() == MENU_TITLE_SCREEN) {
        onTitleScreen = TRUE;
    } else {
        onTitleScreen = FALSE;
    }
    
    player = sCamData->player;
    if (player == NULL) {
        return;
    }
    
    func_80008D90(player->parent);
    CamControl_store_player_coords(player);
    CamControl_average_player_speed(sCamData, player);
    
    //Optionally override the player's position (restored at end of function)
    if (sCamData->setPlayerPosition) {
        player->srt.transl.x = sCamData->newPlayerPosition.x;
        player->srt.transl.y = sCamData->newPlayerPosition.y;
        player->srt.transl.z = sCamData->newPlayerPosition.z;
        player->globalPosition.x = sCamData->newPlayerPosition.x;
        player->globalPosition.y = sCamData->newPlayerPosition.y;
        player->globalPosition.z = sCamData->newPlayerPosition.z;
        sCamData->setPlayerPosition = FALSE;
    }
    
    //Transform the player's coordinates based on their parent Object
    if (player->parent != NULL) {
        transform_point_by_object(
            player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, 
            &player->srt.transl.x, &player->srt.transl.y, &player->srt.transl.z, 
            player->parent
        );
        player->srt.yaw += player->parent->srt.yaw;
    }
    
    //Set up easing into a different camera DLL (if needed)
    if (sEaseSetupNeeded) {
        //If duration is less than 2 frames, skip lerp and arrive at goal immediately
        if (sEaseDuration > 1) {
            tSpeed = 1.0f / sEaseDuration;
            if ((tSpeed <= 0.0f) || (tSpeed > 1.0f)) {
                tSpeed = 1.0f;
            }
            sCamData->tValue = 1.0f;
            sCamData->tSpeed = tSpeed;
            sCamData->lerpFlags = sEaseFlags;
        } else {
            sCamData->tValue = 0.0f;
            sCamData->lerpFlags = CamControl_Ease_None;
        }
        
        //Store the current camera DLL's translation/rotation/fov as the ease startpoint
        if (sCamData->tValue == 1.0f) {
            sCamData->easeStartX = sCamData->srt.transl.x;
            sCamData->easeStartY = sCamData->srt.transl.y;
            sCamData->easeStartZ = sCamData->srt.transl.z;
            sCamData->easeStartYaw = sCamData->srt.yaw;
            sCamData->easeStartPitch = sCamData->srt.pitch;
            sCamData->easeStartRoll = sCamData->srt.roll;
            sCamData->goalFov = sCamData->fov;
        }
        
        sPreviousID = sActiveID;
        sPreviousFree = sActiveFree;
        sPreviousSetupVal = sActiveSetupVal;
        CamControl_replace_active_module(sNextID, sCamAction);
        sEaseSetupNeeded = FALSE;
        
        if (sCamAction != NULL) {
            mmFree(sCamAction);
            sCamAction = NULL;
        }
    }

    //Advance the camera
    if (sActiveModule != NULL) {
        sActiveModule->dll->vtbl->control(sCamData);
        CamControl_update_camera(sCamData);
    }

    //Update the LockIcon and related flags
    if (onTitleScreen == FALSE) {
        if (sCamData->target == NULL) {
            sCamData->highlight = CamControl_find_highlight_object(sCamData, player);
        }
        
        if (sActiveID != DLL_ID_ATTENTIONCAM1) {
            sCamData->targetFlags &= ~ARROW_FLAG_1_Interacted;
        }
    }
    
    sCamData->positionMirror.x = sCamData->srt.transl.x;
    sCamData->positionMirror.y = sCamData->srt.transl.y;
    sCamData->positionMirror.z = sCamData->srt.transl.z;
    sCamData->highlightFlags = 0;

    //Restore the player's transl/globalPosition
    CamControl_restore_player_coords(player);
}

// offset: 0x57C | func: 2 | export: 2
CamControl_Data* CamControl_get_data(void) {
    return sCamData;
}

// offset: 0x598 | func: 3 | export: 3
/** Returns the DLL ID of the current camera module */
s32 CamControl_get_dll_ID(void) {
    return sActiveID;
}

// offset: 0x5B4 | func: 4 | export: 4
CamControl_Module* CamControl_get_active_module(void) {
    return sActiveModule;
}

// offset: 0x5D0 | func: 5 | export: 5
CamControl_Module* CamControl_get_camnormal_module(void) {
    s32 i;

    for (i = 0; i < sCamModuleCount; i++){
        if (sCamModules[i]->id == DLL_ID_CAMNORMAL) {
            return sCamModules[i];
        }
    }
    
    return NULL;
}

// offset: 0x630 | func: 6 | export: 6
//TODO: unsure of type for arg4 - sometimes CameraAction*, other times something else (8 bytes long)?
void CamControl_change_camera_module(s32 dllID, s32 doDeferredFree, s32 setupVal, s32 actionSize, void* action, s32 easeDuration, u8 easeFlags) {
    void* currentCamAction;

    //Free the previously-used camera action
    currentCamAction = sCamAction;
    if (currentCamAction != NULL) {
        mmFree(currentCamAction);
        sCamAction = NULL;
        sEaseSetupNeeded = FALSE;
    }
    
    //Set up the camera DLL to ease into
    sNextID = dllID;
    sEaseDuration = easeDuration;
    
    if (action != NULL) {
        sCamAction = mmAlloc(actionSize, ALLOC_TAG_CAM_COL, 0);
        bcopy(action, sCamAction, actionSize);
    } else {
        sCamAction = NULL;
    }
    
    sNextFree = doDeferredFree;
    sNextSetupVal = setupVal;

    sEaseSetupNeeded = TRUE;
    sEaseFlags = easeFlags;
}

// offset: 0x738 | func: 7 | export: 26
/** Switches back to the previously-used camera module */
void CamControl_revert_camera_module(s32 easeDuration, u8 easeFlags) {
    if (sPreviousID != DLL_NONE) {
        CamControl_change_camera_module(sPreviousID, sPreviousFree, sPreviousSetupVal, 0, NULL, easeDuration, easeFlags);
    }
}

// offset: 0x7B4 | func: 8 | export: 22
void CamControl_func_7B4(UNK_PTR* arg0, s32 arg1) { //TODO: figure out type for arg0 (it's CameraAction* sometimes, but f32* when called in SB_Galleon)
    if (sActiveModule != NULL) {
        sActiveModule->dll->vtbl->func3(arg0, arg1);
    }
}

// offset: 0x804 | func: 9 | export: 7
/** Returns a pointer to a `CameraAction` read from CAMERAACTIONS.BIN */
CameraAction* CamControl_get_camera_action(s32 actionIndex) {
    void* camAction;

    if (actionIndex == 0) {
        return NULL;
    }
    actionIndex--;
    
    camAction = mmAlloc(sizeof(CameraAction), ALLOC_TAG_CAM_COL, 0);
    queue_load_file_region_to_ptr(
        camAction, 
        CAMACTIONS_BIN, 
        actionIndex * sizeof(CameraAction), 
        sizeof(CameraAction)
    );
    return camAction;
}

// offset: 0x888 | func: 10 | export: 8
void CamControl_change_mode(u32 cameraMode, s32 params) {
    Unk_DLL2_Func888 actionLockOn; //TO-DO: figure out what this is/how to handle it (different struct from CameraAction? 8 bytes long)
    Unk_DLL2_Func888 actionStatic;
    CameraAction* camAction;
    s32 pad[2];
    
    switch (cameraMode) {
    case Camera_MODE_1_Lock_On:
        actionLockOn.unk0 = params & 0x7F; //extract lower bits
        actionLockOn.unk4 = params & 0x80; //store uppermost bit
        CamControl_change_camera_module(DLL_ID_CAMLOCKON, 1, 0, sizeof(Unk_DLL2_Func888), &actionLockOn, 120, 0xFF);
        break;
    case Camera_MODE_2_Static:
        actionStatic.unk0 = params & 0x7F; //extract lower bits
        actionStatic.unk4 = params & 0x80; //store uppermost bit
        CamControl_change_camera_module(DLL_ID_CAMSTATIC, 1, 0, sizeof(Unk_DLL2_Func888), &actionStatic, 120, 0xFF);
        break;
    case Camera_MODE_3_Normal:
        CamControl_change_camera_module(DLL_ID_CAMNORMAL, 0, 1, 0, NULL, 120, 0xFF);
        break;
    case Camera_MODE_4_Module:
        CamControl_change_camera_module(DLL_ID_CAMNORMAL + params, 1, 0, 0, NULL, 120, 0xFF);
        break;
    case Camera_MODE_0_CameraAction:
    default:
        if (params != 0) {
            camAction = CamControl_get_camera_action(params);
        }
        if (camAction != NULL) {
            gDLL_29_Gplay->vtbl->func_F60()->unk12 = params;
            if ((sActiveID == DLL_ID_CAMSHIPBATTLE1) || (sActiveID == DLL_ID_CAMTALK1)) {
                CamControl_get_camnormal_module()->dll->vtbl->func3(camAction, 16);
            } else {
                if ((camAction->unk0 == 0) || (camAction->unk0 != 1)) {
                    CamControl_change_camera_module(DLL_ID_CAMNORMAL, 0, 2, sizeof(CameraAction), camAction, 0, 0xFF);
                } else {
                    //If camAction->unk0 == 1 (1st-person mode locked out?)
                    CamControl_change_camera_module(DLL_ID_CAM93, 1, 2, sizeof(CameraAction), camAction, 0, 0xFF);
                }
            }
            mmFree(camAction);
        }
        break;
    }
}

// offset: 0xB08 | func: 11 | export: 9
void CamControl_store_player(Object* player, s32 arg1) {
    if (player != sCamData->player) {
        sCamData->player = player;
    }
}

// offset: 0xB38 | func: 12 | export: 10
void CamControl_reposition_player(f32 x, f32 y, f32 z) {
    sCamData->setPlayerPosition = TRUE;
    sCamData->newPlayerPosition.x = x;
    sCamData->newPlayerPosition.y = y;
    sCamData->newPlayerPosition.z = z;
}

// offset: 0xB80 | func: 13 | export: 11
/** Shifts the camera by a position delta */
void CamControl_move_camera_by_delta(f32 dx, f32 dy, f32 dz) {
    sCamData->srt.transl.x += dx;
    sCamData->srt.transl.y += dy;
    sCamData->srt.transl.z += dz;
}

// offset: 0xBD4 | func: 14 | export: 12
void CamControl_func_BD4(f32 xNumerator, Vec4f* vec4, f32 xDivisor, f32 xMin, f32 y, f32 w) {
    f32 x;

    x = xNumerator / xDivisor;
    if (x < xMin) {
        x = xMin;
    }
    
    vec4->x = x;
    vec4->y = y;
    vec4->z = 0.0f;
    vec4->w = w;
}

// offset: 0xC28 | func: 15 | export: 13
/** 
  * Stores the components of a player-to-camera vector onto the dx/dy/dz f32* arguments,
  * and optionally stores the absolute lateral distance as well (clamped to a minimum of 5).
  * 
  * The yOffset argument can be used to sample the vector from a point above the player's feet.
  *
  * NOTE: arg0 could also be an Object*, or a pointer to an SRT.
  */
void CamControl_get_player_to_camera_distances(Camera* camera, f32* dx, f32* dy, f32* dz, f32* distance2D, f32 yOffset) {
    Object* player;

    player = sCamData->player;
    
    *dx = camera->srt.transl.x - player->srt.transl.x;
    *dy = camera->srt.transl.y - (player->srt.transl.y + yOffset);
    *dz = camera->srt.transl.z - player->srt.transl.z;
    
    //Optional: store lateral distance
    if (distance2D != NULL) {
        *distance2D = SQ(*dx) + SQ(*dz);
        if (*distance2D > 0.0f) {
            *distance2D = sqrtf(*distance2D);
        }

        //Clamp min value to 5
        if (*distance2D < 5.0f) {
            *distance2D = 5.0f;
        }
    }
}

// offset: 0xD10 | func: 16 | export: 14
/** Returns the Object the LockIcon is currently locked onto */
Object* CamControl_get_target_object(void) {
    return sCamData->target;
}

// offset: 0xD30 | func: 17 | export: 15
/** Returns the Object the LockIcon is hovering over (not necessarily Z-targetting) */
Object* CamControl_get_highlighted_object(void) {
    return sCamData->highlight;
}

// offset: 0xD50 | func: 18 | export: 17
void CamControl_set_target_object(Object* target) {
    sCamData->target = target;
    sCamData->highlight = target;
}

// offset: 0xD7C | func: 19 | export: 18
int CamControl_has_interpolation_finished(void) {
    return (sCamData->tValue > 0.0f);
}

// offset: 0xDB8 | func: 20 | export: 19
void CamControl_lock_icon_tick(void) {
    Object* hlObject;
    Object* arrow;
    Object* player;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSquared;
    s32 opacity;
    u8 targetFlags;
    ObjectStruct74* targetCoords;

    arrow = sLockIcon;
    hlObject = sCamData->highlight;
    player = get_player();
    
    if (menu_get_current() == MENU_TITLE_SCREEN) {
        return;
    }
    
    sCamData->targetFlags &= ~ARROW_FLAG_4_Highlighted;
    
    if ((player == NULL) || (player->stateFlags & OBJSTATE_IN_SEQ)) {
        sIconState = LockIcon_STATE_Hidden;
        return;
    }
    
    if (hlObject != NULL) {
        //Get lateral distance between player and lock-on point
        distSquared = hlObject->unk78[hlObject->unkD4].interactRadius * 4;
        distSquared *= distSquared;

        targetCoords = &hlObject->unk74[hlObject->unkD4];
        dx = player->globalPosition.x - targetCoords->refPoint.x;
        dz = player->globalPosition.z - targetCoords->refPoint.z;
        
        //Check whether to ignore differences in player/target's y coordinates
        if (hlObject->unkAF & ARROW_FLAG_80_Ignore_TranslateY) {
            dy = 0.0f;
        } else {
            dy = player->globalPosition.y - targetCoords->refPoint.y;
        }
        
        //Highlight the target object when in range
        if ((SQ(dx) + SQ(dz) < distSquared) && ((dy > -100.0f) && (dy < 100.0f))) {
            hlObject->unkAF |= ARROW_FLAG_4_Highlighted;
            sCamData->targetFlags |= 4;
        }
        
        //Update target Object's interaction flags when A button pressed
        if (hlObject->unkAF & ARROW_FLAG_4_Highlighted) {
            if (!(hlObject->unkAF & ARROW_FLAG_10_Greyed_Out)){
                if (joy_get_pressed(0) & A_BUTTON) {
                hlObject->unkAF |= ARROW_FLAG_1_Interacted;
                }
            } else if (joy_get_pressed(0) & A_BUTTON) {
                gDLL_6_AMSFX->vtbl->play_sound(hlObject, SOUND_6E6_Interaction_Refused, MAX_VOLUME, 0, 0, 0, 0);
            }
        }
    }
    
    //OBJ_LockIcon State Machine
    switch (sIconState) {
    case LockIcon_STATE_Hidden:
    default:
        sIconRotateSpeed = 0;
        arrow->opacity = OBJECT_OPACITY_MAX;
        if (hlObject != NULL) {
            sIconState = LockIcon_STATE_Highlight_Start;
        }
        break;

    case LockIcon_STATE_Highlight_Start:
        //When a nearby Object starts being highlighted by the LockIcon (playing a little "blip!" sound)
        sIconRotateSpeed = 0;
        arrow->opacity = OBJECT_OPACITY_MAX;
        sIconState = LockIcon_STATE_Highlighted;
        gDLL_6_AMSFX->vtbl->play_sound(hlObject, SOUND_43C_Target_Highlighted, MAX_VOLUME, 0, 0, 0, 0);
        break;

    case LockIcon_STATE_Highlighted:
        //While the LockIcon is highlighting a nearby Object (but it's not targeted yet)
        if ((hlObject == NULL) || (sActiveID == DLL_ID_CAM1STPERSON) || (hlObject->unkAF & (ARROW_FLAG_20_Removed | ARROW_FLAG_8_No_Targetting))) {
            gDLL_6_AMSFX->vtbl->play_sound(hlObject, SOUND_72E_Lock_Disengage, MAX_VOLUME, 0, 0, 0, 0);
            sIconState = LockIcon_STATE_Vanish;
        } else {
            //Automatically lock-on (during Ship Battle?)
            if (sActiveID == DLL_ID_CAMSHIPBATTLE1) {
                gDLL_6_AMSFX->vtbl->play_sound(hlObject, SOUND_72D_Lock_On, MAX_VOLUME, 0, 0, 0, 0);
                sIconState = LockIcon_STATE_Lock_On;
                sIconRapidTimer = 60; //icon spins extra quickly for 1st second
            }
            
            //Decelerate rotation speed
            if (sIconRotateSpeed > 0) {
                sIconRotateSpeed -= gUpdateRate * 8;
            }
            break;
        }
        break;
    case LockIcon_STATE_Lock_On:
        //Disengage lock when needed
        if ((hlObject == NULL) || (hlObject->unkAF & (ARROW_FLAG_20_Removed | ARROW_FLAG_8_No_Targetting))) {
            gDLL_6_AMSFX->vtbl->play_sound(hlObject, SOUND_72E_Lock_Disengage, MAX_VOLUME, 0, 0, 0, 0);
            sIconState = LockIcon_STATE_Vanish;
            break;
        }
        
        if (sActiveID != DLL_ID_CAMSHIPBATTLE1) {
            sIconState = LockIcon_STATE_Highlighted;
        }
        
        hlObject->unkAF |= ARROW_FLAG_2_Targeted;
        
        //Handle icon rotation speed (fast at first, then slows to lower speed)
        if (sIconRapidTimer > 0) {
            sIconRapidTimer -= gUpdateRate;
            sIconRotateSpeed = 0x400;
        } else {
            if (sIconRotateSpeed > 0x200) {
                sIconRotateSpeed -= gUpdateRate * 8;
            }
            if (sIconRotateSpeed < 0x200) {
                sIconRotateSpeed = 0x200;
            }
        }
        break;
    case LockIcon_STATE_Vanish:
        //Fade out the LockIcon
        opacity = arrow->opacity - (gUpdateRate * 12);
        if (opacity < 0) {
            opacity = 0;
        }
        arrow->opacity = opacity;
        
        //LockIcon moves upwards while vanishing
        if (arrow->opacity > 0) {
            sIconRapidTimer -= gUpdateRate;
            arrow->srt.transl.y += 2.0f * gUpdateRateF;
            arrow->globalPosition.y += 2.0f * gUpdateRateF;
        } else {
            sIconState = LockIcon_STATE_Hidden;
        }

        //Reappear if an object becomes highlighted midway through the fade-out
        if (hlObject) {
            sIconState = LockIcon_STATE_Highlight_Start;
        }
        break;
    }
    
    //Update rotation
    if (sIconRotateSpeed < 0) {
        sIconRotateSpeed = 0;
    }    
    arrow->srt.yaw += sIconRotateSpeed * gUpdateRate;
}

// offset: 0x1408 | func: 21 | export: 20
void CamControl_lock_icon_print(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols) {
    if (menu_get_current() != MENU_TITLE_SCREEN) {
        _data_0[0] = -1;
        CamControl_print(sCamData->highlight, sActiveID == DLL_ID_CAMSHIPBATTLE1, gdl, mtxs, vtxs, pols);
    }
}

// offset: 0x14A8 | func: 22 | export: 21
void CamControl_set_letterbox_goal(s32 height, s32 startAtGoal) {
    if (sCamData->letterboxGoal < height) {
        sCamData->letterboxGoal = height;
        sCamData->letterboxSpeed = 1;
        
        if (startAtGoal) {
            camera_set_letterbox(height);
        }
    }
}

// offset: 0x1520 | func: 23 | export: 24
/** Can be used to prevent searching for nearby Objects to highlight with the LockIcon (when highlightFlags has bit 2 set) */
void CamControl_apply_highlight_flags(s32 flags) {
    sCamData->highlightFlags |= flags;
}

// offset: 0x1548 | func: 24 | export: 16
/*
 * Sets bit 2 of `camData->targetFlags` (or unsets it if the `enable` argument is false) 
 */
void CamControl_set_target_flag_2(s32 enable) {
    if (enable) {
        sCamData->targetFlags |= 2;
    } else {
        sCamData->targetFlags &= ~2;
    }
}

// offset: 0x1594 | func: 25 | export: 25
/** Can set bits 4 and/or 5 of `sCamData->targetFlags` (affects arrow being greyed out?) */
void CamControl_apply_target_flags(s32 bits4and5) {
    sCamData->targetFlags |= (bits4and5 * 8) & 0x18;
}

// offset: 0x15C4 | func: 26
s32 CamControl_get_loaded_module_index(u16 dllID) {
    s32 i;

    for (i = 0; i < sCamModuleCount; i++){
        if (sCamModules[i]->id == dllID) {
            return i;
        }
    }
    
    return DLL_NONE;
}

// offset: 0x1630 | func: 27
void CamControl_free_module(s32 idx) {
    dll_unload(sCamModules[idx]->dll);
    mmFree(sCamModules[idx]);
    sCamModules[idx] = sCamModules[sCamModuleCount - 1];
    sCamModuleCount--;
}

// offset: 0x16C8 | func: 28
s32 CamControl_load_module(u16 dllID, s32 doDeferredFree) {
    CamControl_Module *camModule;
    CamControl_Module *module;
    
    module = mmAlloc(sizeof(CamControl_Module), ALLOC_TAG_CAM_COL, 0);
    
    sCamModules[sCamModuleCount] = module;
    sCamModuleCount++;
    
    camModule = module;
    camModule->id = dllID;
    camModule->doDeferredFree = doDeferredFree;
    camModule->dll = dll_load_deferred(dllID, 4);
    return sCamModuleCount - 1;
}

// offset: 0x1770 | func: 29
static void CamControl_update_camera(CamControl_Data* camData) {
    Camera* camera;
    f32 tValue;
    Vec4f ease;

    set_camera_selector(0);
    camera = get_main_camera();
    camera->srt.yaw = camData->srt.yaw;
    camera->srt.pitch = camData->srt.pitch;
    camera->srt.roll = camData->srt.roll;
    camera->srt.transl.x = camData->srt.transl.x;
    camera->srt.transl.y = camData->srt.transl.y;
    camera->srt.transl.z = camData->srt.transl.z;

    sFov = camData->fov;

    if (camData->tValue > 0.0f) {
        camData->tValue -= camData->tSpeed * gUpdateRateF;

        //Get Bezier-eased tValue for interpolation
        ease.w = 0.0f;
        ease.z = 0.0f;
        ease.x = 0.0f;
        ease.y = 1.0f;
        tValue = 1.0f - func_80004C5C(&ease, camData->tValue, 0);

        //Linear interpolation (position)
        if (camData->lerpFlags & CamControl_Ease_X) {
            camera->srt.transl.x = camData->easeStartX + (camera->srt.transl.x - camData->easeStartX)*tValue;
        }
        if (camData->lerpFlags & CamControl_Ease_Y) {
            camera->srt.transl.y = camData->easeStartY + (camera->srt.transl.y - camData->easeStartY)*tValue;
        }
        if (camData->lerpFlags & CamControl_Ease_Z) {
            camera->srt.transl.z = camData->easeStartZ + (camera->srt.transl.z - camData->easeStartZ)*tValue;
        }
        
        //Linear interpolation (rotation)
        if (camData->lerpFlags & CamControl_Ease_Yaw) {
            camData->dYaw = camData->easeStartYaw - (camera->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(camData->dYaw)
            camera->srt.yaw = camData->easeStartYaw - ((s16)(camData->dYaw * tValue));
        }
        if (camData->lerpFlags & CamControl_Ease_Pitch) {
            camData->dPitch = camData->easeStartPitch - (camera->srt.pitch & 0xFFFF);
            CIRCLE_WRAP(camData->dPitch)
            camera->srt.pitch = camData->easeStartPitch - ((s16)(camData->dPitch * tValue));
        }
        if (camData->lerpFlags & CamControl_Ease_Roll) {
            camData->dRoll = camData->easeStartRoll - (camera->srt.roll & 0xFFFF);
            CIRCLE_WRAP(camData->dRoll)
            camera->srt.roll = camData->easeStartRoll - ((s16)(camData->dRoll * tValue));
        }
    }
    
    //Change FOV
    if (camera_get_fov() != sFov) {
        camera_set_fov(sFov);
    }
    
    update_camera_for_object(camera);
    map_func_80046B58(camera->tx, camera->ty, camera->tz);

    //Update camera letterboxing
    sLetterboxHeight = camera_get_letterbox();
    if (sLetterboxHeight != camData->letterboxGoal) {
        if (sLetterboxHeight < camData->letterboxGoal) {
            sLetterboxHeight += camData->letterboxSpeed * (s32) gUpdateRateF;
            if (camData->letterboxGoal < sLetterboxHeight) {
                sLetterboxHeight = camData->letterboxGoal;
            }
        } else {
            sLetterboxHeight -= camData->letterboxSpeed * (s32) gUpdateRateF;
            if (sLetterboxHeight < camData->letterboxGoal) {
                sLetterboxHeight = camData->letterboxGoal;
            }
        }
        camera_set_letterbox(sLetterboxHeight);
    }
    
    camData->letterboxGoal = 0;
}

// offset: 0x1BD0 | func: 30
void CamControl_replace_active_module(u16 camDLLID, CameraAction* camAction) {   
    //Call the existing camera module's free method (if it's a different DLL ID)
    if ((sActiveModule != NULL) && (sActiveID != camDLLID)){
        sActiveModule->dll->vtbl->free(sCamData);
        if (sActiveModule->doDeferredFree == TRUE){
            CamControl_free_module(sActiveLoadedIndex);
            sActiveModule = NULL;
            sActiveID = DLL_NONE;
            sActiveLoadedIndex = DLL_NONE;
        }
    }
    
    //Load the new camera module DLL (or reuse it if it's already loaded)
    sActiveLoadedIndex = CamControl_get_loaded_module_index(camDLLID);
    if (sActiveLoadedIndex == DLL_NONE) {
        sActiveLoadedIndex = CamControl_load_module(camDLLID, sNextFree);
    }
    
    //Call the new camera DLL's setup method, and set the current module to the next module
    if (sActiveLoadedIndex != DLL_NONE) {
        sActiveModule = sCamModules[sActiveLoadedIndex];
        sActiveID = sActiveModule->id;
        sActiveModule->dll->vtbl->setup(sCamData, sNextSetupVal, camAction);
    } else {
        sActiveModule = NULL;
        sActiveID = DLL_NONE;
    }
    
    sActiveFree = sNextFree;
    sActiveSetupVal = sNextSetupVal;
}

// offset: 0x1D94 | func: 31
/** Stores a copy of the player's transl/globalPosition near the start of a CamControl tick */
void CamControl_store_player_coords(Object* player) {
    sPlayerX = player->srt.transl.x;
    sPlayerY = player->srt.transl.y;
    sPlayerZ = player->srt.transl.z;
    sPlayerGlobalPositionX = player->globalPosition.x;
    sPlayerGlobalPositionY = player->globalPosition.y;
    sPlayerGlobalPositionZ = player->globalPosition.z;
}

// offset: 0x1DF0 | func: 32
/** Restores the player's original transl/globalPosition near the end of a CamControl tick */
void CamControl_restore_player_coords(Object* player) {
    Object* parent = player->parent;
    
    player->srt.transl.x = sPlayerX;
    player->srt.transl.y = sPlayerY;
    player->srt.transl.z = sPlayerZ;
    player->globalPosition.x = sPlayerGlobalPositionX;
    player->globalPosition.y = sPlayerGlobalPositionY;
    player->globalPosition.z = sPlayerGlobalPositionZ;
    
    if (parent != NULL) {
        player->srt.yaw -= parent->srt.yaw;
    }
}

// offset: 0x1E64 | func: 33
/** Averages the player's speed over the past 5 frames, stored to `speedAverage` (absolute value) */
void CamControl_average_player_speed(CamControl_Data* camData, Object* player) {
    f32 speedMagnitude;
    s32 i;

    //Copy speed samples up one slot
    for (i = 0; i < ARRAYCOUNT_S(camData->speedSamples) - 1; i++) {
        camData->speedSamples[i] = camData->speedSamples[i + 1];
    }

    //Put player's absolute lateral speed into last slot
    speedMagnitude = SQ(player->velocity.x) + SQ(player->velocity.z);
    if (speedMagnitude > 0.0f) {
        speedMagnitude = sqrtf(speedMagnitude);
    }
    camData->speedSamples[4] = speedMagnitude;

    //Average the 5 speed samples
    camData->speedAverage = 0.0f;
    for (i = 0; i < ARRAYCOUNT_S(camData->speedSamples); i++) {
        camData->speedAverage += camData->speedSamples[i];
    }
    camData->speedAverage *= 0.2f;
    
    if (camData->speedAverage < 0.0f) {
        camData->speedAverage = -camData->speedAverage;
    }
}

// offset: 0x1F8C | func: 34
/** Searches through an array of highlightable Objects, finding which (if any) to display the LockIcon highlight over */
Object* CamControl_find_highlight_object(CamControl_Data* camData, Object* player) {
    Object* objects[8]; //targettable Objects
    s32 matchCount;
    s32 i;
    s32 temp;
    s32 lockIndex;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 dYaw;
    Object* obj;
    f32 distSquared;
    s32 count;
    Vec3s16 sp74;
    Vec3s16 sp6C;
    ObjectStruct74* targetCoords;
    ObjectStruct78* targetDef;
    s32 angularRange;

    //Return early if flag set
    if (camData->highlightFlags & 2) {
        return NULL;
    }
    
    count = gDLL_1_cmdmenu->vtbl->get_target_objects(objects, ARRAYCOUNT(objects), 0xFF, 1, 1000.0f);

    for (i = 0, matchCount = 0; i < count; i++){
        obj = objects[i];

        if ((obj->opacity != 0) && !(obj->srt.flags & OBJFLAG_INVISIBLE) && !(obj->stateFlags & OBJSTATE_DESTROYED)) {
            if (obj->unkAF & (ARROW_FLAG_20_Removed | ARROW_FLAG_8_No_Targetting)){
                continue;
            }

            targetCoords = obj->unk74;
            targetDef = obj->unk78;
                
            if (obj->unkD4 < obj->def->numLockdata) {
                lockIndex = obj->unkD4;
            } else {
                lockIndex = 0;
            }
                
            if (!targetCoords) {
                continue;
            }
            
            dx = player->globalPosition.x - targetCoords[lockIndex].drawPoint.x;
            dz = player->globalPosition.z - targetCoords[lockIndex].drawPoint.z;
            if (obj->unkAF & ARROW_FLAG_80_Ignore_TranslateY) {
                dy = 0.0f;
            } else {
                dy = player->globalPosition.y - targetCoords[lockIndex].drawPoint.y;
            }

            distSquared = targetDef[lockIndex].hlRadius * 4;
            distSquared *= distSquared;
            
            if (((SQ(dx) + SQ(dz)) < distSquared) && (dy > -100.0f) && (dy < 100.0f)) {
                if (sActiveID == DLL_ID_CAMSHIPBATTLE1) {
                    angularRange = 0x8000;
                } else {
                    angularRange = (s16)(targetDef[lockIndex].hlAngularRange * 182.04f);
                }

                dYaw = player->srt.yaw - (arctan2s(dx, dz) & 0xFFFF);
                CIRCLE_WRAP(dYaw)
                
                if ((dYaw < angularRange) && (-angularRange < dYaw)) {
                    if (targetDef[lockIndex].flags & 0x20) {
                        //Use VoxMaps (check if player has line-of-sight to Object, maybe?)
                        func_80007EE0((Vec3f*)&targetCoords[lockIndex], &sp74);
                        func_80007EE0(&player->srt.transl, &sp6C);
                        sp6C.y += 2;
                        if (func_80008048(&sp74, &sp6C, 0, 0, 0) != 0) {
                            objects[matchCount] = obj;
                            matchCount++;
                        }
                    } else {
                        objects[matchCount] = obj;
                        matchCount++;
                    }
                }
            }
        }
    }

    if (camData->lockObjIndex >= matchCount) {
        camData->lockObjIndex = 0;
    }
    
    if (matchCount) {
        return objects[camData->lockObjIndex];
    }

    return NULL;  
}

// offset: 0x236C | func: 35
/** Creates an `OBJ_LockIcon` and stores a reference to it in `sLockIcon` */
void CamControl_create_LockIcon(void) {
    sLockIcon = obj_create(
        obj_alloc_setup(sizeof(ObjSetup), OBJ_LockIcon),
        0,
        -1,
        -1,
        0
    );
}

// offset: 0x23D0 | func: 36
/** Draws the `OBJ_LockIcon` over the highlighted Object */
void CamControl_print(Object* obj, s32 isCamShipBattle, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols) {
    Object *lockIcon;
    s32 modelIndex;
    u8 colourIndex;
    u8 lockIndex;
    u8 textIdx;
    ObjectStruct74 *lockCoords;
    
    lockIcon = sLockIcon;
    
    //Don't draw the LockIcon if it's hidden
    if ((lockIcon->opacity == 0) || 
        !(sIconState == LockIcon_STATE_Highlighted || 
          sIconState == LockIcon_STATE_Lock_On || 
          sIconState == LockIcon_STATE_Vanish
        )
    ) {
        return;
    }
    
    if (obj) {
        if (!obj->unk74) {
            return;
        }

        lockCoords = &obj->unk74[lockIndex = obj->unkD4];
        colourIndex = obj->unk78[lockIndex].flags;
        modelIndex = ((colourIndex & 0xF) & 0xFFFF) - 1;
        
        if (modelIndex < 0) {
            modelIndex *= 0;
        }
        
        if ((obj->unkAF & ARROW_FLAG_10_Greyed_Out) || (!(sCamData->targetFlags & ARROW_FLAG_4_Highlighted))) {
            modelIndex += 6;
        }
        
        textIdx = obj->unkD8;
        
        if (textIdx >= 4) {
            textIdx = 0;
        }
        
        _data_0[0] = obj->def->gametextIndex[textIdx];
        lockIcon->globalPosition.x = lockCoords->drawPoint.x;
        lockIcon->globalPosition.y = lockCoords->drawPoint.y;
        lockIcon->globalPosition.z = lockCoords->drawPoint.z;
        lockIcon->modelInstIdx = modelIndex;
        lockIcon->parent = obj->parent;
        
        if (lockIcon->parent != 0){
            inverse_transform_point_by_object(
                lockIcon->globalPosition.x, lockIcon->globalPosition.y, lockIcon->globalPosition.z, 
                &lockIcon->srt.transl.x, &lockIcon->srt.transl.y, &lockIcon->srt.transl.z, 
                lockIcon->parent
            );
        } else {
            lockIcon->srt.transl.x = lockIcon->globalPosition.x;
            lockIcon->srt.transl.y = lockIcon->globalPosition.y;
            lockIcon->srt.transl.z = lockIcon->globalPosition.z;
        }
    }
    
    lockIcon->srt.pitch = 0;
    lockIcon->srt.roll = 0;
    lockIcon->srt.scale = 0.08f;
    lockIcon->opacityWithFade = lockIcon->opacity;

    draw_object(lockIcon, gdl, mtxs, vtxs, pols, 1.0f);
    
    lockIcon->modelInsts[lockIcon->modelInstIdx]->unk34 &= ~8;
}

// offset: 0x25D4 | func: 37 | export: 23
s16 CamControl_get_target_gametextID(void) {
    return _data_0[0];
}
