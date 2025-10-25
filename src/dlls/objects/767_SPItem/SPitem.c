#include "common.h"
#include "sys/gfx/model.h"

#include "dlls/objects/214_animobj.h"
#include "dlls/objects/768_SPshop.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef struct {
    Object* shop;           //SPShop
    s16 gametextLineIdx;    //Descriptive text's string index in Gametext file #3
} SPItem_Data;

typedef struct {
    ObjSetup base;
    u8 modelIndex;
    u8 itemIndex;
    u8 yaw;
    u8 pitch;
} SPItem_Setup;

static int SPItem_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);
static void SPItem_bought_callback(Object* self, s32 arg1, s32 arg2);

// offset: 0x0 | ctor
void SPItem_ctor(void *dll) { }

// offset: 0xC | dtor
void SPItem_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SPItem_setup(Object* self, SPItem_Setup* objSetup, s32 arg2) {
    self->unkB0 |= 0x2000;
    self->animCallback = (AnimationCallback)SPItem_anim_callback;

    self->modelInstIdx = objSetup->modelIndex;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch << 8;

    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
}

// offset: 0x84 | func: 1 | export: 1
void SPItem_control(Object* self) {
    s32 scarabCount;
    s32 initialPrice;
    SPItem_Setup* objSetup;
    Object* player;
    SPItem_Data* objData;
    f32 distance;

    objSetup = (SPItem_Setup*)self->setup;
    player = get_player();
    objData = self->data;
    distance = 10000.0f;

    //Get SPShop object
    if (objData->shop == NULL) {
        objData->shop = obj_get_nearest_type_to(0xA, self, &distance);
        if (objData->shop) {
            //Hide item if not in stock or already purchased
            if (((DLL_768_SPShop*)objData->shop->dll)->vtbl->SPShop_is_item_shown(objData->shop, objSetup->itemIndex) == FALSE || 
                (((DLL_768_SPShop*)objData->shop->dll)->vtbl->SPShop_is_item_hidden(objData->shop, objSetup->itemIndex))) {
                self->srt.flags |= 0x4000;  //don't draw
                self->unkB0 |= 0x8000;    //don't animate
                self->unkAF |= 8;         //don't allow targetting
            }
            //Get gametext line index
            objData->gametextLineIdx = ((DLL_768_SPShop*)objData->shop->dll)->vtbl->SPShop_get_item_gametext_index(objData->shop, objSetup->itemIndex);
        }
    //Check if A pressed while target overhead
    } else if (self->unkAF & 1) {
        scarabCount = ((DLL_Unknown*)player->dll)->vtbl->func[20].withOneArgS32((s32)player);
        initialPrice = ((DLL_768_SPShop*)objData->shop->dll)->vtbl->SPShop_get_initial_price(objData->shop, objSetup->itemIndex);
        ((DLL_768_SPShop*)objData->shop->dll)->vtbl->SPShop_set_current_item_index(objData->shop, objSetup->itemIndex);
        
        //Play sequence: "Yoooouuu pay this much!"
        if (scarabCount >= initialPrice) {
            if (player->id == OBJ_Krystal) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
            } else {
                gDLL_3_Animation->vtbl->func17(2, self, -1);
            }
        //Play sequence: "Put that dooown, you don't have enough Scarabs!"
        } else if (player->id == OBJ_Krystal) {
            gDLL_3_Animation->vtbl->func17(1, self, -1);
        } else {
            gDLL_3_Animation->vtbl->func17(3, self, -1);
        }
        set_button_mask(0, A_BUTTON);
    }

    func_80024108(self, 0.005f, delayFloat, NULL);
}

// offset: 0x350 | func: 2 | export: 2
void SPItem_update(Object *self) { }

// offset: 0x35C | func: 3 | export: 3
void SPItem_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
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
        gDLL_17->vtbl->func1(self, 0x136, &trans, 4, -1, NULL);
    }

    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}


// offset: 0x440 | func: 4 | export: 4
void SPItem_free(Object *self, s32 a1) {
}

// offset: 0x450 | func: 5 | export: 5
s32 SPItem_get_model_flags(Object* self) {
    SPItem_Setup* objSetup = (SPItem_Setup*)self->setup;
    return (objSetup->modelIndex << 11) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x468 | func: 6 | export: 6
u32 SPItem_get_data_size(Object *self, u32 a1) {
    return sizeof(SPItem_Data);
}

// offset: 0x47C | func: 7
static int SPItem_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    //Set end-of-sequence callback
    animObjData->unkF4 = &SPItem_bought_callback; 
    return 0;
}

// offset: 0x4AC | func: 8
void SPItem_bought_callback(Object* self, s32 arg1, s32 arg2) {
    SPItem_Data *objData;
    SPItem_Setup *objSetup;
    Object *shop;
        
    objData = self->data;
    objSetup = (SPItem_Setup*)self->setup;
    shop = objData->shop;

    //Check if object should be hidden
    if (((DLL_768_SPShop*) shop->dll)->vtbl->SPShop_is_item_hidden(shop, objSetup->itemIndex)){
        self->srt.flags |= 0x4000;  //don't draw
        self->unkB0 |= 0x8000;    //don't update animation
        self->unkAF |= 8;         //don't allow targetting
    }
    
    //Clear SPShop DLL's current item index
    shop = objData->shop;
    ((DLL_768_SPShop*)shop->dll)->vtbl->SPShop_set_current_item_index(shop, -1);
}
