#include "common.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/768_SPshop.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/124_SPItem.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef struct {
    Object* shop;           //SPShop
    s16 gametextLineIdx;    //Descriptive text's string index in Gametext file #3
} SPItem_Data;

static int SPItem_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);
static void SPItem_boughtCallback(Object* self, Object* override, AnimObj_Data* animObjData);

// offset: 0x0 | ctor
void SPItem_ctor(void* dll) { }

// offset: 0xC | dtor
void SPItem_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SPItem_obj_Setup(Object* self, SPItem_Setup* objSetup, s32 reset) {
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    self->animCallback = SPItem_animCallback;

    self->modelInstIdx = objSetup->modelIndex;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch << 8;

    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
}

// offset: 0x84 | func: 1 | export: 1
void SPItem_obj_Control(Object* self) {
    s32 scarabCount;
    s32 initialPrice;
    SPItem_Setup* objSetup;
    Object* player;
    SPItem_Data* objData;
    f32 distance;

    objSetup = (SPItem_Setup*)self->setup;
    player = objGetPlayer();
    objData = self->data;
    distance = 10000.0f;

    //Get SPShop object
    if (objData->shop == NULL) {
        objData->shop = objGetNearestTypeTo(OBJTYPE_LevelControl, self, &distance);
        if (objData->shop) {
            //Hide item if not in stock or already purchased
            if (dll_SPShop(objData->shop)->IsItemShown(objData->shop, objSetup->itemIndex) == FALSE || 
                (dll_SPShop(objData->shop)->IsItemHidden(objData->shop, objSetup->itemIndex))) {
                self->srt.flags |= OBJFLAG_INVISIBLE;
                self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
                self->unkAF |= ARROW_FLAG_8_No_Targetting;
            }
            //Get gametext line index
            objData->gametextLineIdx = dll_SPShop(objData->shop)->GetItemGametextIndex(objData->shop, objSetup->itemIndex);
        }
    //Check if A pressed while target overhead
    } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
        scarabCount = ((DLL_210_Player*)player->dll)->vtbl->get_scarabs(player);
        initialPrice = dll_SPShop(objData->shop)->GetInitialPrice(objData->shop, objSetup->itemIndex);
        dll_SPShop(objData->shop)->SetCurrentItemIndex(objData->shop, objSetup->itemIndex);
        
        //Play sequence: "Yoooouuu pay this much!"
        if (scarabCount >= initialPrice) {
            if (player->id == OBJ_Krystal) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
            }
        //Play sequence: "Put that dooown, you don't have enough Scarabs!"
        } else if (player->id == OBJ_Krystal) {
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        } else {
            gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
        }
        joyDisableButtons(0, A_BUTTON);
    }

    objAnimAdvance(self, 0.005f, gUpdateRateF, NULL);
}

// offset: 0x350 | func: 2 | export: 2
void SPItem_update(Object* self) { }

// offset: 0x35C | func: 3 | export: 3
void SPItem_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    SRT trans;

    if (!visibility) {
        return;
    }

    //Special case: magic gems
    if (self->id == OBJ_SPMagic) {
        if (self->modelInstIdx == 0) {
            trans.roll = 0x160;
            trans.pitch = 0xF;
        } else {
            trans.roll = 0xDF;
            trans.pitch = 0x12;
        }
        //Create sparkle particles
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_136, &trans, PARTFXFLAG_4, -1, NULL);
    }

    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
}


// offset: 0x440 | func: 4 | export: 4
void SPItem_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x450 | func: 5 | export: 5
s32 SPItem_obj_GetModelFlags(Object* self) {
    SPItem_Setup* objSetup = (SPItem_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(objSetup->modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x468 | func: 6 | export: 6
u32 SPItem_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(SPItem_Data);
}

// offset: 0x47C | func: 7
static int SPItem_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue) {
    //Set end-of-sequence callback
    animObjData->unkF4 = SPItem_boughtCallback; 
    return 0;
}

// offset: 0x4AC | func: 8
void SPItem_boughtCallback(Object* self, Object* override, AnimObj_Data* animObjData) {
    SPItem_Data* objData;
    SPItem_Setup* objSetup;
    Object* shop;
        
    objData = self->data;
    objSetup = (SPItem_Setup*)self->setup;
    shop = objData->shop;

    //Check if object should be hidden
    if (dll_SPShop(shop)->IsItemHidden(shop, objSetup->itemIndex)){
        self->srt.flags |= OBJFLAG_INVISIBLE;
        self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    
    //Clear SPShop DLL's current item index
    shop = objData->shop;
    dll_SPShop(shop)->SetCurrentItemIndex(shop, -1);
}
