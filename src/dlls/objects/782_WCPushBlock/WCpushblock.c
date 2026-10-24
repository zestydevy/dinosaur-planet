#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/779_WCLevelControl.h"
#include "dlls/objects/782_WCPushBlock.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/main.h"
#include "types.h"

typedef struct {
/*000*/    DLL27_Data unk0;   //Unused: probably DLL27_Data
/*260*/    Object* levelCtrl; //WCLevelControl
/*264*/    f32 limitX;
/*268*/    f32 limitZ;
/*26C*/    u32 soundHandle;
/*270*/    s16 gridX;
/*272*/    s16 gridZ;
/*274*/    u8 state;
/*275*/    u8 moveDirection;
/*276*/    u8 puzzlePieceID;  //The pushblock's identifier, used by WCLevelControl to quickly store which puzzle element is in each grid cell
/*277*/    u8 collidedType;
} WCPushBlock_Data;

typedef enum {
    WCPushBlock_DIRECTION_X_Positive, //Moving forwards along world X axis
    WCPushBlock_DIRECTION_X_Negative, //Moving backward along world X axis
    WCPushBlock_DIRECTION_Z_Positive, //Moving forwards along world Z axis
    WCPushBlock_DIRECTION_Z_Negative  //Moving backward along world Z axis
} WCPushBlock_MoveDirections;

static s32 WCPushBlock_appearIfPlayerOutsideDestination(Object* self, WCPushBlock_Data* objdata, Object* player);

// offset: 0x0 | ctor
void WCPushBlock_ctor(void* dll){ }

// offset: 0xC | dtor
void WCPushBlock_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void WCPushBlock_obj_Setup(Object* self, WCPushBlock_Setup* setup, s32 reset) {
    WCPushBlock_Data* objdata = self->data;

    self->opacity = 0;
    self->modelInstIdx = setup->modelIndex;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = WCPushBlock_MODELIDX_Moon;
    }
    objdata->puzzlePieceID = setup->puzzlePieceID;
}

// offset: 0x58 | func: 1 | export: 1
void WCPushBlock_obj_Control(Object* self) {
    TextureAnimator* texAnim;
    Object* player;
    f32 speed;
    s32 opacity;
    s32 isNighttime;
    f32 distance;
    WCPushBlock_Data* objdata;
    f32 time;
    s32 stopped;

    objdata = self->data;
    player = objGetPlayer();
    distance = 100000.0f;
    if (objdata->levelCtrl == NULL) {
        objdata->levelCtrl = objGetNearestTypeTo(OBJTYPE_LevelControl, self, &distance); //getting WCLevelControl
        self->opacity = 0;
        return;
    }
    
    texAnim = objExprGetTexAnimator(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->frame = 0;
    }

    if (objdata->state != WCPushBlock_STATE_6_Puzzle_Complete) {
        isNighttime = gDLL_7_Newday->vtbl->func8(&time);
        
        if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
            if (mainGetBits(BIT_WC_Sun_Aperture_Opened)) {
                objdata->state = WCPushBlock_STATE_6_Puzzle_Complete;
                dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetupPositionFinished(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
                dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &self->srt.transl.x, &self->srt.transl.z);
            } else if (isNighttime) {
                objdata->state = WCPushBlock_STATE_3_Pushed_to_Bounds;
            }
        } else {
            if (mainGetBits(BIT_WC_Moon_Aperture_Opened)) {
                objdata->state = WCPushBlock_STATE_6_Puzzle_Complete;
                dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetupPositionFinished(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
                dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &self->srt.transl.x, &self->srt.transl.z);
            } else if (isNighttime == FALSE) {
                objdata->state = WCPushBlock_STATE_3_Pushed_to_Bounds;
            }
        }
    }

    switch (objdata->state) {
    case WCPushBlock_STATE_0_Initialising:
        if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
            dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
            dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &self->srt.transl.x, &self->srt.transl.z);
        } else {
            dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
            dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &self->srt.transl.x, &self->srt.transl.z);
        }
        objdata->state = WCPushBlock_STATE_1_Pushable;
        break;
    case WCPushBlock_STATE_1_Pushable:
        //Fade in
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > OBJECT_OPACITY_MAX) {
            opacity = OBJECT_OPACITY_MAX;
        }
        self->opacity = opacity;

        self->velocity.x = 0.0f;
        self->velocity.z = 0.0f;

        if (((DLL_210_Player*)player->dll)->vtbl->func47(player, self, &objdata->moveDirection)) {
            if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
                if (objdata->moveDirection == WCPushBlock_DIRECTION_X_Positive) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, -1, 0);
                }
                else if (objdata->moveDirection == WCPushBlock_DIRECTION_X_Negative) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, 1, 0);
                }
                else if (objdata->moveDirection == WCPushBlock_DIRECTION_Z_Positive) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, 0, -1);
                }
                else if (objdata->moveDirection == WCPushBlock_DIRECTION_Z_Negative) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, 0, 1);
                }
            } else {
                if (objdata->moveDirection == WCPushBlock_DIRECTION_X_Positive) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, -1, 0);
                }
                else if (objdata->moveDirection == WCPushBlock_DIRECTION_X_Negative) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, 1, 0);
                }
                else if (objdata->moveDirection == WCPushBlock_DIRECTION_Z_Positive) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, 0, -1);
                }
                else if (objdata->moveDirection == WCPushBlock_DIRECTION_Z_Negative) {
                    objdata->collidedType = dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleMove(
                        self, objdata->gridX, objdata->gridZ, &objdata->limitX, &objdata->limitZ, 0, 1);
                }
            }

            if ((objdata->limitX != self->srt.transl.x) || (objdata->limitZ != self->srt.transl.y)) {
                dll_amSfx->Play(self, SOUND_9B9_Block_Shifting, 1, &objdata->soundHandle, 0, 0, 0);
                objdata->state = WCPushBlock_STATE_2_Moving;
            }
        }
        break;
    case WCPushBlock_STATE_2_Moving:
        speed = sqrtf(SQ(self->velocity.x) + SQ(self->velocity.z));
        speed -= 0.25f;
        if (speed < 0.0f) {
            speed = 0.0f;
        }
        speed = ((126.0f * speed) / 1.25f) + 1.0f;
        if (speed > 127.0f) {
            speed = 127.0f;
        }
        
        dll_amSfx->SetVol(objdata->soundHandle, speed);
        objMove(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);

        stopped = FALSE;
        if (objdata->moveDirection == WCPushBlock_DIRECTION_X_Positive) {
            if (self->velocity.x < 1.5f) {
                self->velocity.x += (gUpdateRateF * 0.05f);
            }
            if (self->srt.transl.x >= objdata->limitX) {
                self->srt.transl.x = objdata->limitX;
                stopped = TRUE;
            }
        } else if (objdata->moveDirection == WCPushBlock_DIRECTION_X_Negative) {
            if (self->velocity.x > -1.5f) {
                self->velocity.x -= (gUpdateRateF * 0.05f);
            }
            if (self->srt.transl.x <= objdata->limitX) {
                self->srt.transl.x = objdata->limitX;
                stopped = TRUE;
            }
        } else if (objdata->moveDirection == WCPushBlock_DIRECTION_Z_Positive) {
            if (self->velocity.z < 1.5f) {
                self->velocity.z += (gUpdateRateF * 0.05f);
            }
            if (self->srt.transl.z >= objdata->limitZ) {
                self->srt.transl.z = objdata->limitZ;
                stopped = TRUE;
            }
        } else if (objdata->moveDirection == WCPushBlock_DIRECTION_Z_Negative) {
            if (self->velocity.z > -1.5f) {
                self->velocity.z -= (gUpdateRateF * 0.05f);
            }
            if (self->srt.transl.z <= objdata->limitZ) {
                self->srt.transl.z = objdata->limitZ;
                stopped = TRUE;
            }
        }

        //Clamp velocity
        if (self->velocity.x > 1.5f) {
            self->velocity.x = 1.5f;
        }
        if (self->velocity.x < -1.5f) {
            self->velocity.x = -1.5f;
        }
        if (self->velocity.z > 1.5f) {
            self->velocity.z = 1.5f;
        }
        if (self->velocity.z < -1.5f) {
            self->velocity.z = -1.5f;
        }

        if (stopped) {
            dll_amSfx->Stop(objdata->soundHandle);
            self->velocity.x = 0.0f;
            self->velocity.z = 0.0f;

            if (objdata->collidedType == WCBlockPuzzle_HIT_Symbol) {
                objdata->state = WCPushBlock_STATE_4_In_Correct_Spot;
                dll_amSfx->Play(self, SOUND_9BB_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, 0, 0, 0);
                if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
                    mainIncrementBits(BIT_WC_Sun_Pushblock_Puzzle_Progress);
                } else {
                    mainIncrementBits(BIT_WC_Moon_Pushblock_Puzzle_Progress);
                }
            } else if (objdata->collidedType == WCBlockPuzzle_HIT_Pushblock) {
                objdata->state = WCPushBlock_STATE_1_Pushable;
                dll_amSfx->Play(self, SOUND_9BA_Thud, MAX_VOLUME, NULL, 0, 0, 0);
            } else {
                objdata->state = WCPushBlock_STATE_3_Pushed_to_Bounds;
                dll_amSfx->Play(self, SOUND_9BA_Thud, MAX_VOLUME, NULL, 0, 0, 0);
            }

            if (objdata->state != WCPushBlock_STATE_3_Pushed_to_Bounds) {
                if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
                    dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCell(0, objdata->gridX, objdata->gridZ);
                    dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetGridPositionFromCoords(self, self->srt.transl.x, self->srt.transl.z, &objdata->gridX, &objdata->gridZ);
                    dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCell(objdata->puzzlePieceID, objdata->gridX, objdata->gridZ);
                } else {
                    dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCell(0, objdata->gridX, objdata->gridZ);
                    dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetGridPositionFromCoords(self, self->srt.transl.x, self->srt.transl.z, &objdata->gridX, &objdata->gridZ);
                    dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCell(objdata->puzzlePieceID, objdata->gridX, objdata->gridZ);
                }
            }
        }
        break;
    case WCPushBlock_STATE_3_Pushed_to_Bounds:
        func_800267A4(self);

        if (self->opacity == OBJECT_OPACITY_MAX) {
            dll_amSfx->Play(self, SOUND_9C5_Vanish, MAX_VOLUME, NULL, 0, 0, 0);
        }

        //Fade out
        opacity = self->opacity - (gUpdateRate * 8);
        if (opacity < 0) {
            opacity = 0;
        }
        self->opacity = opacity;

        //Reappear once the player moves away from the spot the block should appear
        if (self->opacity == 0 && WCPushBlock_appearIfPlayerOutsideDestination(self, objdata, objGetPlayer())) {
            if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
                dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
                dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &self->srt.transl.x, &self->srt.transl.z);
            } else {
                dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
                dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &self->srt.transl.x, &self->srt.transl.z);
            }
            objdata->state = WCPushBlock_STATE_5_Reappearing;
        }
        break;
    case WCPushBlock_STATE_5_Reappearing:
        if (self->opacity == 0) {
            func_8002674C(self);
            dll_amSfx->Play(self, SOUND_9C6_Appear, MAX_VOLUME, NULL, 0, 0, 0);
        }
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > OBJECT_OPACITY_MAX) {
            opacity = OBJECT_OPACITY_MAX;
        }
        self->opacity = opacity;
        if (self->opacity >= OBJECT_OPACITY_MAX) {
            objdata->state = WCPushBlock_STATE_1_Pushable;
        }
        break;
    case WCPushBlock_STATE_6_Puzzle_Complete:
        self->opacity = OBJECT_OPACITY_MAX;
        /* fallthrough */
    case WCPushBlock_STATE_4_In_Correct_Spot:
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            texAnim->frame = 0x100;
        }
        break;
    }
}

// offset: 0xF38 | func: 2 | export: 2
void WCPushBlock_obj_Update(Object* self) { }

// offset: 0xF44 | func: 3 | export: 3
void WCPushBlock_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF98 | func: 4 | export: 4
void WCPushBlock_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0xFA8 | func: 5 | export: 5
u32 WCPushBlock_obj_GetModelFlags(Object* self) {
    WCPushBlock_Setup* setup;
    s8 modelIndex;

    setup = (WCPushBlock_Setup*)self->setup;
    modelIndex = setup->modelIndex;
    if (modelIndex >= self->def->numModels) {
        modelIndex = WCPushBlock_MODELIDX_Moon;
    }
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0xFDC | func: 6 | export: 6
u32 WCPushBlock_obj_GetDataSize(Object* self, s32 offsetAddr) {
    return sizeof(WCPushBlock_Data);
}

// offset: 0xFF0 | func: 7
static s32 WCPushBlock_appearIfPlayerOutsideDestination(Object* self, WCPushBlock_Data* objdata, Object* player) {
    f32 positionX;
    f32 positionZ;
    f32 min;
    f32 max;
    f32 time;
    u8 isNighttime;

    isNighttime = gDLL_7_Newday->vtbl->func8(&time);

    if (self->modelInstIdx == WCPushBlock_MODELIDX_Sun) {
        //Sun block
        if (isNighttime) {
            return FALSE;
        }
        dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
        dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &positionX, &positionZ);
    } else {
        //Moon block
        if (isNighttime == FALSE) {
            return FALSE;
        }

        dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->gridX, &objdata->gridZ);
        dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCoordsFromGridPosition(self, objdata->gridX, objdata->gridZ, &positionX, &positionZ);
    }

    //Check if player out of range
    max = positionX + 56.0f;
    min = positionX - 56.0f;
    if (max < player->srt.transl.x || player->srt.transl.x < min) {
        return TRUE;
    }

    max = positionZ + 56.0f;
    min = positionZ - 56.0f;
    if (max < player->srt.transl.z || player->srt.transl.z < min) {
        return TRUE;
    }

    return FALSE;
}

