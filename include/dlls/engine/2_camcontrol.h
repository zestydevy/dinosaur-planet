#ifndef _DLLS_2_H
#define _DLLS_2_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "sys/camera.h"
#include "types.h"

typedef enum {
    Camera_MODE_0_CameraAction = 0,
    Camera_MODE_1_Lock_On = 1,
    Camera_MODE_2_Static = 2,
    Camera_MODE_3_Normal = 3,
    Camera_MODE_4_Module = 4
} CameraModes;

typedef enum {
    CamControl_Ease_None = 0,
    CamControl_Ease_Yaw = 1,
    CamControl_Ease_Pitch = 2,
    CamControl_Ease_Roll = 4,
    CamControl_Ease_X = 8,
    CamControl_Ease_Y = 0x10,
    CamControl_Ease_Z = 0x20
} CamControl_EaseFlags;

typedef struct {
/*000*/    SRT srt;                 //Camera transform
/*018*/    Vec3f initialCoords;     //Camera's initial coordinates
/*024*/    s8 unk24[0xA0 - 0x24];
/*0A0*/    Object* player;
/*0A4*/    Vec3f positionMirror;    //Mirror of camera's position
/*0B0*/    f32 fov;                 //Field of view
/*0B4*/    f32 speedAverage;        //Player's average lateral speed over the past 5 frames
/*0B8*/    f32 speedSamples[5];     //Player lateral speed samples
/*0C8*/    Vec3f newPlayerPosition; //Where to move player when `setPlayerPosition` is nonzero
/*0D8*/    s8 unkD8[0xE4 - 0xD8];
/*0E4*/    f32 tValue;      //Bezier ease tValue
/*0E8*/    f32 tSpeed;      //Rate of change of tValue
/*0EC*/    f32 unkEC;
/*0F0*/    s16 dYaw;        //Range for lerping
/*0F2*/    s16 dPitch;      //Range for lerping
/*0F4*/    s16 dRoll;       //Range for lerping
/*0F6*/    s16 easeStartYaw;     //Goal for lerping
/*0F8*/    s16 easeStartPitch;   //Goal for lerping
/*0FA*/    s16 easeStartRoll;    //Goal for lerping
/*0FC*/    f32 easeStartX;       //Goal for lerping
/*100*/    f32 easeStartY;       //Goal for lerping
/*104*/    f32 easeStartZ;       //Goal for lerping
/*108*/    f32 goalFov;     //Goal for lerping (arrives at goal immediately)
/*10C*/    Object* target;     //Object the player is Z-targetting
/*110*/    Object* highlight;  //Object the LockIcon is hovering over (not necessarily Z-targetting yet)
/*114*/    u8 lockObjIndex;    //The index of the Object the LockIcon is hovering over (out of the list of nearby targettable Objects)
/*115*/    s8 letterboxGoal;   //Height to ease towards
/*116*/    s8 letterboxSpeed;  //Rate of change of letterbox height
/*117*/    u8 setPlayerPosition;  //Changes the player's location to `newPlayerPosition` when nonzero
/*118*/    s8 unk118;
/*119*/    u8 lerpFlags;          //Which SRT components to interpolate
/*11A*/    u8 highlightFlags;     //Can disable LockIcon Object highlighting (stops searching for nearby highlightable Objects)
/*11B*/    u8 targetFlags;        //Affects LockIcon being greyed out (Seems to use same flags as Object->unkAF, and generally mirrors unkAF value of Object being highlighted)
} CamControl_Data;

typedef struct {
    u16 id;             //DLL ID
    DLL_Unknown* dll;   //Camera module DLL (CAMNORMAL, ATTENTIONCAM1, CAM1STPERSON, etc.)
    u8 doDeferredFree;  //frees cam module DLL when set to 1 
} CamControl_Module;

typedef struct {
	s8 unk0; //disables first-person view when set
	s8 unk1; //transition duration
	s8 unk2; //fov
	s8 unk3; //player distance min
	s8 unk4; //player distance max
	s8 unk5; //height, or maybe angle - yaw/roll? (maybe s16?)
	s8 unk6;
	s8 unk7; //framebuffer/letterbox-related
	s8 unk8; //interpolation-related?
	s8 unk9; //interpolation-related?
	s8 unkA; //interpolation-related?
	s8 unkB; //max lateral speed?
	s8 unkC; //initial speed?
	s8 unkD; //pad?
	s8 unkE; //pad?
	s8 unkF; //pad?
} CameraAction;

DLL_INTERFACE(DLL_2_camera) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*init_data)(Object* player, f32 initialX, f32 initialY, f32 initialZ); //Zeroes CamControl's data, stores a reference to the player Object, and stores the camera's initial position.
/*1*/ void (*tick)(u8 updateRate);							//Main CamControl function (NOTE: actual function doesn't seem to take any arguments, though it's called using `updateRate`)
/*2*/ CamControl_Data* (*get_data)(void);					//Returns a pointer to the CamControl DLL's main data struct
/*3*/ s32 (*get_dll_ID)(void); 								//Returns the DLL ID of the current camera module
/*4*/ CamControl_Module (*get_active_module)(void);			//Returns the CamControl_Module that's currently in use
/*5*/ CamControl_Module (*get_camnormal_module)(void);		//Returns the CamControl_Module for DLL 84: CAMNORMAL (if it's loaded)
/*6*/ void (*change_camera_module)(s32 dllID, s32 doDeferredFree, s32 setupVal, s32 actionSize, void* action, s32 easeDuration, u8 easeFlags);	
/*7*/ CameraAction* (*get_camera_action)(s32 actionIndex);	//Returns a pointer to a `CameraAction` read from CAMERAACTIONS.BIN
/*8*/ void (*change_mode)(u32 cameraMode, s32 index); 		//Can apply a CameraAction (or a different 8-byte struct) depending on the mode value. Depending on mode `params` can be the camera DLL index (relative to CAMNORMAL's ID), or the index of the CameraAction to use, or params for the 8-byte struct. TO-DO: update description once better understood!
/*9*/ void (*store_player)(Object* player, s32 arg1); 		//Stores a reference to the player Object onto CamControl_Data 
/*10*/ void (*reposition_player)(f32 x, f32 y, f32 z);		//Changes the player's SRT transl and positionMirror (the next time export1 runs)
/*11*/ void (*move_camera_by_delta)(f32 dx, f32 dy, f32 dz); //Shifts the camera by a position delta
/*12*/ void (*func12)(f32 xNumerator, Vec4f* vec4, f32 xDivisor, f32 xMin, f32 y, f32 w); //TO-DO: figure out what this is for (related to interpolation ease tangents, maybe?)
/*13*/ void (*get_player_to_camera_distances)(Camera* camera, f32* dx, f32* dy, f32* dz, f32* distance2D, f32 yOffset); //Stores the components of a player-to-camera vector onto the dx/dy/dz f32* arguments, and optionally stores the absolute lateral distance as well (clamped to a minimum of 5). The yOffset argument can be used to start the vector from a point above the player's feet. NOTE: arg0 could also be an Object*, or a pointer to an SRT.
/*14*/ Object *(*get_target_object)(void); 					//Returns the Object the LockIcon is currently locked onto
/*15*/ Object *(*get_highlighted_object)(void); 			//Returns the Object the LockIcon is hovering over (not necessarily Z-targetting)
/*16*/ int *(*set_target_flag_2)(s32 enable); 				//Sets bit 2 of `camData->targetFlags` (or unsets it if the `enable` argument is false)
/*17*/ Object *(*set_target_object)(Object* target); 		//Makes the LockIcon target a specific Object
/*18*/ int *(*has_interpolation_finished)(void);			//Returns whether or not the camera is still interpolating between different modes
/*19*/ void (*lock_icon_tick)(void); 						//Updates the OBJ_LockIcon that appears above highlighted/targeted Objects
/*20*/ void (*lock_icon_print)(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols); //Prints OBJ_LockIcon (while not on the Title Screen)
/*21*/ void (*set_letterbox_goal)(s32 height, s32 startAtGoal);    //Sets a new goal height for the camera's letterboxing to ease towards. The `startAtGoal` arg can force the camera to use the new letterbox height immediately.
/*22*/ void (*func22)(void*, s32); 							//Calls export 3 from the current camera module. TODO: unsure of type for arg0 - sometimes seems to be CameraAction*, other times f32*? Also, figure out what to call this!
/*23*/ s16 (*get_target_gametextID)(void); 					//Returns the gametextID to display while the LockIcon is highlighting something (when holding the R button)
/*24*/ void (*apply_highlight_flags)(s32 flags); 			//Can be used to prevent searching for nearby Objects to highlight with the LockIcon (when highlightFlags has bit 2 set)
/*25*/ void (*apply_target_flags)(s32 bits4and5); 			//Can set bits 4 and/or 5 of `sCamData->targetFlags` (affects LockIcon being greyed out?)
/*26*/ void (*revert_camera_module)(s32 easeDuration, u8 easeFlags); //Switches back to the previously-used camera module
};

#endif //_DLLS_2_H
