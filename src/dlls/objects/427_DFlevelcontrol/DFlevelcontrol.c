#include "dll.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/lfx.h"
#include "sys/envfx.h"
#include "game/gamebits.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/420_DFlevelcontrol.h"

typedef struct {
/*00*/ u8 state;
/*01*/ u8 openedWhirlpoolCave;
/*02*/ u8 mapID;
/*03*/ u8 unk3;
} DFlevelcontrol_Data;

typedef enum {
    DFLevelControl_STATE_0_Shrine_Door_Closed,
    DFLevelControl_STATE_1_Shrine_Door_Unlocked,
    DFLevelControl_STATE_2_Finished
} DFLevelControl_States;

static void DFlevelcontrol_initialise(Object* self);
static void DFlevelcontrol_applyFX(Object* self, s32 arg1);

// offset: 0x0 | ctor
void DFlevelcontrol_ctor(void* dll) { }

// offset: 0xC | dtor
void DFlevelcontrol_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFlevelcontrol_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    DFlevelcontrol_Data* objdata = self->data;

    if (mainGetBits(BIT_DF_Shrine_Door_Opened)) {
        objdata->state = DFLevelControl_STATE_2_Finished;
    } else {
        objdata->state = DFLevelControl_STATE_0_Shrine_Door_Closed;
    }

    objdata->openedWhirlpoolCave = mainGetBits(BIT_DF_Demolition_Cave_Destroyed_Whirlpool_Wall_4);
    
    mainSetBits(BIT_DF_8DE, 1 - objdata->openedWhirlpoolCave);

    objdata->mapID = -1;
}

// offset: 0xC0 | func: 1 | export: 1
void DFlevelcontrol_obj_Control(Object* self) {
    DFlevelcontrol_Data* objdata;
    Object* player;

    objdata = self->data;
    player = objGetPlayer();
    dll_amSfx->WaterFallsControl();

    //Run the level's initialisation function when the player enters the map
    if (objdata->mapID != MAP_DISCOVERY_FALLS) {
        if (mapWorldXZToMapID(player->srt.transl.x, player->srt.transl.z) != MAP_DISCOVERY_FALLS) {
            return;
        }

        DFlevelcontrol_initialise(self);
    }
    objdata->mapID = mapWorldXZToMapID(player->srt.transl.x, player->srt.transl.z);

    if ((objdata->openedWhirlpoolCave == FALSE) && (mainGetBits(BIT_DF_Demolition_Cave_Destroyed_Whirlpool_Wall_4))) {
        mainSetBits(BIT_Kyte_Flight_Curve, 70);
        mainSetBits(BIT_DF_8DE, FALSE);
        objdata->openedWhirlpoolCave = TRUE;
    }

    //Shrine Door State Machine
    switch (objdata->state) {
    case DFLevelControl_STATE_0_Shrine_Door_Closed:
        if (mainGetBits(BIT_DF_Shrine_Door_Light_Activated_One) &&
            mainGetBits(BIT_DF_Shrine_Door_Light_Activated_Two) &&
            mainGetBits(BIT_DF_Shrine_Door_Light_Activated_Three) &&
            mainGetBits(BIT_DF_Shrine_Door_Light_Activated_Four)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, DF_ObjGroup11_Shrine_Door, TRUE);
            objdata->state++;
        }
        break;
    case DFLevelControl_STATE_1_Shrine_Door_Unlocked:
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, DF_ObjGroup11_Shrine_Door)) {
            mainSetBits(BIT_DF_Seq_0035_Shrine_Door_Opens, TRUE);
            objdata->state++;
        }
        break;
    }
}

// offset: 0x2F4 | func: 2 | export: 2
void DFlevelcontrol_obj_Update(Object* self) { }

// offset: 0x300 | func: 3 | export: 3
void DFlevelcontrol_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x354 | func: 4 | export: 4
void DFlevelcontrol_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x364 | func: 5 | export: 5
u32 DFlevelcontrol_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x374 | func: 6 | export: 6
u32 DFlevelcontrol_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFlevelcontrol_Data);
}

// offset: 0x388 | func: 7
void DFlevelcontrol_initialise(Object* self) {
    Object* player = objGetPlayer();

    DFlevelcontrol_applyFX(self, TRUE);

    if (dll_player(player)->get_spirit_bits(player, PLAYER_SPIRIT_1)) {
        mainSetBits(BIT_DF_Returned_From_Shrine_With_Spirit, TRUE);
    }
}

// offset: 0x420 | func: 8
/** 
  * Applies Discovery Falls' envFX and lightFX actions.
  */
void DFlevelcontrol_applyFX(Object* self, s32 arg1) {
    Object* player;

    player = objGetPlayer();
    envfxAction(self, player, 0x35, 0);
    envfxAction(self, player, 0x36, 0);
    envfxAction(self, player, 0x37, 0);
    lfxAction(self, player, 0x24, 0, 0, 0);
}
