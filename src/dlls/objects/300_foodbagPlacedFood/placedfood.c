#include "common.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/objmsg.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/315_sidefoodbag.h"
#include "dlls/objects/common/foodbag.h"
#include "unktypes.h"

typedef struct {
    u8 sequenceWasPlayed;
    u8 unloadTimer;
} PlacedFood_Data;

// offset: 0x0 | ctor
void PlacedFood_ctor(void *dll) { }

// offset: 0xC | dtor
void PlacedFood_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void PlacedFood_setup(Object* self, PlacedFood_Setup* objSetup, s32 reset) {
    PlacedFood_Data* objData;

    objData = self->data;

    objData->sequenceWasPlayed = FALSE;
    self->srt.yaw = get_player()->srt.yaw;

    self->srt.scale = 0.01f;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    switch (self->id) {
    case (OBJ_foodbagNewMeat - 1): //fake?
    case OBJ_foodbagNewMeat:
    case OBJ_foodbagNewFish:
        break;
    case OBJ_foodbagBlueMush:
    case OBJ_foodbagBlueGrub:
    case OBJ_foodbagGreenApp:
    case OBJ_foodbagBrownApp:
    case OBJ_foodbagSmokedFi:
    case OBJ_foodbagOldMeat:
    case OBJ_foodbagRedBean:
    case OBJ_foodbagBrownBea:
    case OBJ_foodbagBlueBean:
    case OBJ_foodbagRedMushr:
    case OBJ_foodbagRedGrub:
    case OBJ_foodbagOldMushr:
    case OBJ_foodbagOldGrub:
        break;
    }
    
    obj_init_mesg_queue(self, 1); 
}

// offset: 0xF8 | func: 1 | export: 1
void PlacedFood_control(Object* self) {
    u32 outMessage;
    Object* foodbag;
    Object* player;
    PlacedFood_Data* objData;

    player = get_player();
    objData = self->data;
    
    if (objData->sequenceWasPlayed == FALSE) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objData->sequenceWasPlayed = TRUE;
        return;
    }
    
    //Unload food after being collected
    if (objData->unloadTimer) {
        objData->unloadTimer += gUpdateRateF;
        if (objData->unloadTimer > 0x80) {
            switch (self->id) {
            case OBJ_foodbagRedApple:
            case OBJ_foodbagGreenBea:
            case OBJ_foodbagNewMeat:
            case OBJ_foodbagNewFish:                             
            case OBJ_foodbagGreenApp:                             
            case OBJ_foodbagBrownApp:                             
            case OBJ_foodbagSmokedFi:                             
            case OBJ_foodbagOldMeat:                             
            case OBJ_foodbagRedBean:                             
            case OBJ_foodbagBrownBea:                             
            case OBJ_foodbagBlueBean:                             
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
                ((DLL_IFoodbag*)foodbag->dll)->vtbl->destroy_placed_food(foodbag, self);
                break;
            case OBJ_foodbagBlueMush:                         
            case OBJ_foodbagRedMushr:                         
            case OBJ_foodbagOldMushr:                         
            case OBJ_foodbagBlueGrub:                         
            case OBJ_foodbagRedGrub:                         
            case OBJ_foodbagOldGrub:                         
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0x10);
                ((DLL_IFoodbag*)foodbag->dll)->vtbl->destroy_placed_food(foodbag, self);
                break;
            }
        }
        return;
    }
    
    //Handle storing the food back into the foodbag when re-collected
    while (obj_recv_mesg(self, &outMessage, NULL, 0)) {
        if (outMessage == 0x7000B) {
            switch (self->id) {
            case OBJ_foodbagGreenApp:                         
            case OBJ_foodbagRedApple:
            case OBJ_foodbagBrownApp:                         
            case OBJ_foodbagNewFish:                         
            case OBJ_foodbagSmokedFi:                         
            case OBJ_foodbagNewMeat:
            case OBJ_foodbagOldMeat:                         
            case OBJ_foodbagGreenBea:
            case OBJ_foodbagRedBean:                         
            case OBJ_foodbagBrownBea:                         
            case OBJ_foodbagBlueBean:                         
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
                switch (self->id) {
                case OBJ_foodbagGreenApp:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
                    break;
                case OBJ_foodbagRedApple:                      
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Red_Apple);
                    break;
                case OBJ_foodbagBrownApp:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Brown_Apple);
                    break;
                case OBJ_foodbagNewFish:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Fish);
                    break;
                case OBJ_foodbagSmokedFi:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Smoked_Fish);
                    break;
                case OBJ_foodbagNewMeat:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Dino_Egg);
                    break;
                case OBJ_foodbagOldMeat:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Moldy_Meat);
                    break;
                case OBJ_foodbagGreenBea:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Bean);
                    break;
                case OBJ_foodbagRedBean:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Red_Bean);
                    break;
                case OBJ_foodbagBrownBea:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Brown_Bean);
                    break;
                case OBJ_foodbagBlueBean:                     
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Blue_Bean);
                    break;
                }
                break;
            case OBJ_foodbagBlueMush:                     
            case OBJ_foodbagRedMushr:                     
            case OBJ_foodbagOldMushr:                     
            case OBJ_foodbagBlueGrub:                     
            case OBJ_foodbagRedGrub:                     
            case OBJ_foodbagOldGrub:                     
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0x10);
                switch (self->id) {
                case OBJ_foodbagBlueMush:                 
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Blue_Mushrooms);
                    break;
                case OBJ_foodbagRedMushr:                 
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Red_Mushrooms);
                    break;
                case OBJ_foodbagOldMushr:                 
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Old_Mushrooms);
                    break;
                case OBJ_foodbagBlueGrub:                 
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Blue_Grubs);
                    break;
                case OBJ_foodbagRedGrub:                 
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Red_Grubs);
                    break;
                case OBJ_foodbagOldGrub:                 
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Old_Grubs);
                    break;
                }
                break;
            }
            objData->unloadTimer = 1;
            return;
        }
    }

    //Handle advancing food's animation (only for grubs)
    switch (self->id) {
    case OBJ_foodbagRedApple:                                  
    case OBJ_foodbagGreenBea:                                 
    case OBJ_foodbagNewMeat:                                 
    case OBJ_foodbagNewFish:                                 
    case OBJ_foodbagBlueMush:                                                                                        
    case OBJ_foodbagGreenApp:                                 
    case OBJ_foodbagBrownApp:                                 
    case OBJ_foodbagSmokedFi:                                 
    case OBJ_foodbagOldMeat:                                 
    case OBJ_foodbagRedBean:                                 
    case OBJ_foodbagBrownBea:                                 
    case OBJ_foodbagBlueBean:                                 
    case OBJ_foodbagRedMushr:                                 
    case OBJ_foodbagOldMushr:                                 
        break;
    case OBJ_foodbagBlueGrub:                                 
    case OBJ_foodbagRedGrub:                                 
    case OBJ_foodbagOldGrub:                                 
        func_80024108(self, 0.01f, gUpdateRateF, NULL);
        break;
    }
    
    //Handle re-collecting the food
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        obj_send_mesg(get_player(), 0x7000A, self, (void*)BIT_ALWAYS_1);
    }
}

// offset: 0x82C | func: 2 | export: 2
void PlacedFood_update(Object *self) { }

// offset: 0x838 | func: 3 | export: 3
void PlacedFood_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x850 | func: 4 | export: 4
void PlacedFood_free(Object *self, s32 onlySelf) { }

// offset: 0x860 | func: 5 | export: 5
u32 PlacedFood_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x870 | func: 6 | export: 6
u32 PlacedFood_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(PlacedFood_Data);
}

// offset: 0x884 | func: 7 | export: 7
u8 PlacedFood_get_unload_timer_value(Object* self, UNK_TYPE_32 arg1) {
    PlacedFood_Data* objData = self->data;
    return objData->unloadTimer;
}
