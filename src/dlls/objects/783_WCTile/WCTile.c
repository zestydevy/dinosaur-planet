#include "dlls/objects/779_WCLevelControl.h"
#include "game/gamebits.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/main.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    Object* levelCtrl;
    u8 _unk4[0x8 - 0x4];
    s16 puzzleGridX;
    s16 puzzleGridZ;
    s16 puzzlePieceID;
    s16 state;
} WCTile_Data;

typedef enum {
    WCTile_STATE_0_Initialising,
    WCTile_STATE_1_Ready,
    WCTile_STATE_2_Covered_With_Block,
    WCTile_STATE_3_Fading_Out,
    WCTile_STATE_4_Fading_In,
    WCTile_STATE_5_Puzzle_Finished
} WCTile_States;

typedef enum {
    WCTile_MODELIDX_Moon,
    WCTile_MODELIDX_Sun
} WCTile_ModelIndices;

// offset: 0x0 | ctor
void WCTile_ctor(void* dll) { }

// offset: 0xC | dtor
void WCTile_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCTile_obj_Setup(Object* self, WCTile_Setup* setup, s32 reset) {
    WCTile_Data* objdata = self->data;

    self->srt.transl.y = setup->base.y + 25.0f;

    self->modelInstIdx = setup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = WCTile_MODELIDX_Moon;
    }

    objdata->puzzlePieceID = setup->puzzlePieceID;

    self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    
    self->opacity = 0;
}

// offset: 0x88 | func: 1 | export: 1
void WCTile_obj_Control(Object* self) {
    f32 distance;
    f32 time;
    WCTile_Data* objdata;
    u8 isNight;
    s32 opacity;

    distance = 100000.0f;
    objdata = self->data;

    if (objdata->levelCtrl == NULL) {
        objdata->levelCtrl = objGetNearestTypeTo(OBJTYPE_LevelControl, self, &distance);
        self->opacity = 0;
        return;
    }

    self->srt.yaw += (s16) (gUpdateRateF * 180.0f);

    if (objdata->state != WCTile_STATE_5_Puzzle_Finished) {
        isNight = gDLL_7_Newday->vtbl->func8(&time);
        if (self->modelInstIdx == WCTile_MODELIDX_Sun) {
            if (mainGetBits(BIT_WC_Sun_Aperture_Opened)) {  //Completed Sun Block Puzzle
                objdata->state = WCTile_STATE_5_Puzzle_Finished;
            } else if (isNight) {
                objdata->state = WCTile_STATE_3_Fading_Out;
            }
        } else {
            if (mainGetBits(BIT_WC_Moon_Aperture_Opened)) { //Completed Moon Block Puzzle
                objdata->state = WCTile_STATE_5_Puzzle_Finished;
            } else if (!isNight) {
                objdata->state = WCTile_STATE_3_Fading_Out;
            }
        }
    }

    switch (objdata->state) {
    case WCTile_STATE_0_Initialising:
        if (self->modelInstIdx == WCTile_MODELIDX_Sun) {
            dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->puzzleGridX, &objdata->puzzleGridZ);
            dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCoordsFromGridPosition(self, objdata->puzzleGridX, objdata->puzzleGridZ, &self->srt.transl.x, &self->srt.transl.z);
        } else {
            dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->puzzleGridX, &objdata->puzzleGridZ);
            dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCoordsFromGridPosition(self, objdata->puzzleGridX, objdata->puzzleGridZ, &self->srt.transl.x, &self->srt.transl.z);
        }
        objdata->state = WCTile_STATE_1_Ready;
        break;
    case WCTile_STATE_2_Covered_With_Block:
        self->opacity = 0;
        break;
    case WCTile_STATE_5_Puzzle_Finished:
        self->opacity = 0;
        break;
    case WCTile_STATE_3_Fading_Out:
        opacity = self->opacity - (gUpdateRate * 8);
        if (opacity < 0) {
            opacity = 0;
        }
        self->opacity = opacity;

        if (self->opacity == 0) {
            if (self->modelInstIdx == WCTile_MODELIDX_Sun) {
                if (!isNight) {
                    dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->puzzleGridX, &objdata->puzzleGridZ);
                    dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleSetCoordsFromGridPosition(self, objdata->puzzleGridX, objdata->puzzleGridZ, &self->srt.transl.x, &self->srt.transl.z);
                    objdata->state = WCTile_STATE_4_Fading_In;
                }
            } else {
                if (isNight) {
                    dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetupPositionInitial(objdata->puzzlePieceID, &objdata->puzzleGridX, &objdata->puzzleGridZ);
                    dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleSetCoordsFromGridPosition(self, objdata->puzzleGridX, objdata->puzzleGridZ, &self->srt.transl.x, &self->srt.transl.z);
                    objdata->state = WCTile_STATE_4_Fading_In;
                }
            }
        }
        break;
    case WCTile_STATE_4_Fading_In:
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > 200) {
            opacity = 200;
        }
        self->opacity = opacity;

        if (self->opacity >= 200) {
            objdata->state = WCTile_STATE_1_Ready;
        }
        break;
    case WCTile_STATE_1_Ready:
    default:
        //Continue fading in
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > 200) {
            opacity = 200;
        }
        self->opacity = opacity;

        //Change state when covered up by a pushblock
        if (self->modelInstIdx == WCTile_MODELIDX_Sun) {
            if (objdata->puzzlePieceID != dll_WCLevelControl(objdata->levelCtrl)->SunPuzzleGetCell(objdata->puzzleGridX, objdata->puzzleGridZ)) {
                objdata->state = WCTile_STATE_2_Covered_With_Block;
            }
        } else {
            if (objdata->puzzlePieceID != dll_WCLevelControl(objdata->levelCtrl)->MoonPuzzleGetCell(objdata->puzzleGridX, objdata->puzzleGridZ)) {
                objdata->state = WCTile_STATE_2_Covered_With_Block;
            }
        }
        break;
    }
}

// offset: 0x510 | func: 2 | export: 2
void WCTile_obj_Update(Object* self) { }

// offset: 0x51C | func: 3 | export: 3
void WCTile_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x570 | func: 4 | export: 4
void WCTile_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x580 | func: 5 | export: 5
u32 WCTile_obj_GetModelFlags(Object* self) {
    WCTile_Setup* setup = (WCTile_Setup*)self->setup;
    s8 modelIdx = setup->modelIdx;
    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5B4 | func: 6 | export: 6
u32 WCTile_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCTile_Data);
}
