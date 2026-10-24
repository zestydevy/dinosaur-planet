#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/animseq.h"
#include "sys/joypad.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "sys/objanim.h"
#include "sys/objprint.h"
#include "types.h"
#include "dll.h"

typedef struct {
    u8 state;
    u8 numMushroomsEaten;
    u8 unk2;
    u8 unk3;
} SHqueenearthwalker_Data;

typedef enum {
    SHqueenearthwalker_STATE_1_Initial = 1,
    SHqueenearthwalker_STATE_2_Before_Meeting_Sabre,
    SHqueenearthwalker_STATE_3_Needing_White_Mushrooms,
    SHqueenearthwalker_STATE_4_Healed,
    SHqueenearthwalker_STATE_5_Discussing_Force_Point_Temples,
    SHqueenearthwalker_STATE_6_Discussing_SpellStones,
    SHqueenearthwalker_STATE_7_After_SpellStone_Sequence
} SHqueenearthwalker_States;

typedef enum {
    SHqueenearthwalker_ANIMCMD_3_Show_VFPT = 3,
    SHqueenearthwalker_ANIMCMD_4_Show_SpellStone = 4
} SHqueenearthwalker_AnimMessages;

#define DEBUG_SHORTEN_MUSHROOM_QUEST

#ifdef DEBUG_SHORTEN_MUSHROOM_QUEST
#define WHITE_MUSHROOMS_NEEDED 1 //@debug code: shortens the mushroom quest!
#else
#define WHITE_MUSHROOMS_NEEDED 10
#endif

static int SHqueenearthwalker_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void SHqueenearthwalker_ctor(void *dll) { }

// offset: 0xC | dtor
void SHqueenearthwalker_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHqueenearthwalker_obj_Setup(Object* self, Generic_Setup* setup, s32 reset) {
    SHqueenearthwalker_Data* objdata;

    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    self->animCallback = SHqueenearthwalker_animCallback;
    objdata->numMushroomsEaten = mainGetBits(BIT_SH_Queen_EW_White_Mushrooms_Eaten);
    objdata->state = mainGetBits(BIT_SH_Queen_EW_Quest_Progress);
}

// offset: 0xA0 | func: 1 | export: 1
void SHqueenearthwalker_obj_Control(Object* self) {
    SHqueenearthwalker_Data* objdata;
    s32 prevState;

    objdata = self->data;
    prevState = objdata->state;
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    //Handle animation
    {
        if (self->curModAnimId != 1) {
            objAnimSet(self, 1, 0.0f, 0);
        }
        objAnimAdvance(self, 0.005f, gUpdateRate, NULL);
    }

    //State Machine
    switch (objdata->state) {
    case SHqueenearthwalker_STATE_1_Initial:
        //Advance state automatically
        objdata->state = SHqueenearthwalker_STATE_2_Before_Meeting_Sabre;
        break;
    case SHqueenearthwalker_STATE_2_Before_Meeting_Sabre:
        //Advance to the White Mushrooms quest state when talked to
        if (self->unkAF & ARROW_FLAG_1_Interacted) {
            joyDisableButtons(0, A_BUTTON);
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            mainSetBits(BIT_SH_Move_Thorntail_Blocking_Hollow_Log, 1);
            objdata->state = SHqueenearthwalker_STATE_3_Needing_White_Mushrooms;
        }
        break;
    case SHqueenearthwalker_STATE_3_Needing_White_Mushrooms:
        if (self->unkAF & ARROW_FLAG_4_Highlighted) {
            //Check if White Mushrooms were offered
            if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_Inventory_White_Mushrooms)) {
                joyDisableButtons(0, A_BUTTON);
                objdata->numMushroomsEaten += mainGetBits(BIT_Inventory_White_Mushrooms);

                if (objdata->numMushroomsEaten < WHITE_MUSHROOMS_NEEDED) { //@debug code: only 1 mushroom needed, not 10!
                    //Play a mushroom eating sequence
                    gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
                } else {
                    //Finish the quest after enough White Mushrooms have been fed
                    objdata->state = SHqueenearthwalker_STATE_4_Healed;
                    dll_task->MarkTaskCompleted(TASK_NUM(GAMETEXT_0FF_Task_011));
                    mainSetBits(BIT_SH_Move_Thorntail_Blocking_Swapstone, 1);
                }

                mainSetBits(BIT_Inventory_White_Mushrooms, 0);
                mainSetBits(BIT_SH_Queen_EW_White_Mushrooms_Eaten, objdata->numMushroomsEaten);
            } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
                //Chat with the player
                joyDisableButtons(0, A_BUTTON);
                gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            }
        }
        break;
    case SHqueenearthwalker_STATE_4_Healed:
        //Start a chain of sequences where the Queen asks Sabre to save Dinosaur Planet
        gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
        break;
    case SHqueenearthwalker_STATE_5_Discussing_Force_Point_Temples:
        gDLL_3_Animation->vtbl->start_obj_sequence(6, self, -1);
        break;
    case SHqueenearthwalker_STATE_6_Discussing_SpellStones:
        gDLL_3_Animation->vtbl->start_obj_sequence(7, self, -1);
        objdata->state = SHqueenearthwalker_STATE_7_After_SpellStone_Sequence;
        break;
    case SHqueenearthwalker_STATE_7_After_SpellStone_Sequence:
        break;
    default:
        //Advance state automatically
        objdata->state = SHqueenearthwalker_STATE_1_Initial;
        break;
    }

    if (prevState != objdata->state) {
        mainSetBits(BIT_SH_Queen_EW_Quest_Progress, objdata->state);
    }
}

// offset: 0x464 | func: 2 | export: 2
void SHqueenearthwalker_obj_Update(Object *self) { }

// offset: 0x470 | func: 3 | export: 3
void SHqueenearthwalker_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C4 | func: 4 | export: 4
void SHqueenearthwalker_obj_Free(Object *self, s32 onlySelf) { }

// offset: 0x4D4 | func: 5 | export: 5
u32 SHqueenearthwalker_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x4E4 | func: 6 | export: 6
u32 SHqueenearthwalker_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(SHqueenearthwalker_Data);
}

// offset: 0x4F8 | func: 7
static int SHqueenearthwalker_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    SHqueenearthwalker_Data* objdata;
    s32 prevState;
    s32 i;

    objdata = self->data;
    prevState = objdata->state;
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    animData->unk62 = 0;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case SHqueenearthwalker_ANIMCMD_3_Show_VFPT:
            objdata->state = SHqueenearthwalker_STATE_5_Discussing_Force_Point_Temples;
            gDLL_29_Gplay->vtbl->set_act(MAP_VOLCANO_FORCE_POINT_TEMPLE, 11);
            mainSetBits(BIT_Play_Seq_0298_Queen_Shows_VFPT, 1);
            mapWarpPlayer(WARP_VFP_CALDERA_LOWER, /*fadeToBlack=*/FALSE);
            break;
        case SHqueenearthwalker_ANIMCMD_4_Show_SpellStone:
            objdata->state = SHqueenearthwalker_STATE_6_Discussing_SpellStones;
            gDLL_29_Gplay->vtbl->set_act(MAP_VOLCANO_FORCE_POINT_TEMPLE, 11);
            mainSetBits(BIT_Play_Seq_0299_Queen_Shows_SpellStones, 1);
            mapWarpPlayer(WARP_VFP_CALDERA_LOWER, /*fadeToBlack=*/FALSE);
            break;
        }
    }

    if (prevState != objdata->state) {
        mainSetBits(BIT_SH_Queen_EW_Quest_Progress, objdata->state);
    }
    
    return 0;
}
