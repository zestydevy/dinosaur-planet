#include "common.h"

#include "dlls/engine/56_putdown.h"
#include "dlls/objects/314_foodbag.h"

typedef struct {
    ObjSetup base;
} SideFoodbag_Setup;

typedef struct {
    DLL_56_Putdown *dllPutdown;         //Handles removing/updating food bag items
    FoodbagGamebits capacityGamebits;   //Bag size gamebits
    u16 capacity;                       //Current capacity
    GplayStruct14* bagTimers;           //Expiry timers for bag items
    FoodbagPlaced placedObjects;        //Food placed in world
} SideFoodbag_Data;

/*0x0*/ static FoodbagItem dino_foodbag_items[] = {
    {0,     0, 0, 0,  NO_GAMEBIT,                   NO_FOOD_OBJECT_ID}, 
    {2000,  2, 1, 4,  BIT_Dino_Bag_Blue_Mushrooms,  OBJ_foodbagBlueMush},
    {6000,  2, 1, 4,  BIT_Dino_Bag_Red_Mushrooms,   OBJ_foodbagBlueMush},
    {6000,  1, 1, 0,  BIT_Dino_Bag_Old_Mushrooms,   OBJ_foodbagBlueMush},
    {6000,  2, 1, 32, BIT_Dino_Bag_Blue_Grubs,      OBJ_foodbagBlueGrub},
    {6000,  2, 1, 32, BIT_Dino_Bag_Red_Grubs,       OBJ_foodbagBlueGrub},
    {30000, 1, 1, 0,  BIT_Dino_Bag_Old_Grubs,       OBJ_foodbagBlueGrub},
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM
};
/*0x78*/ static s32 dino_foodbag_gamebitIDs[] = {
    BIT_Dino_Bag_Blue_Mushrooms,
    BIT_Dino_Bag_Red_Mushrooms, 
    BIT_Dino_Bag_Old_Mushrooms, 
    BIT_Dino_Bag_Blue_Grubs, 
    BIT_Dino_Bag_Red_Grubs, 
    BIT_Dino_Bag_Old_Grubs
};

void SideFoodbag_set_capacity(Object* self);
void SideFoodbag_func_530(Object* self, s32 arg1);
void SideFoodbag_func_594(Object* self, s16 arg1);

// offset: 0x0 | ctor
void SideFoodbag_ctor(void *dll) { }

// offset: 0xC | dtor
void SideFoodbag_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SideFoodbag_setup(Object* self, SideFoodbag_Setup *objSetup, s32 arg2) {
    s32 pad[2];
    s32 playerNo;
    SideFoodbag_Data* objData;
    s32 index;

    objData = self->data;
    playerNo = gDLL_29_Gplay->vtbl->get_playerno();
    objData->dllPutdown = dll_load_deferred(56, 10);
    objData->placedObjects.nextIndex = 0;
    
    for (index = 0; index < 20; index++){
        objData->placedObjects.objects[index] = NULL;
        objData->placedObjects.foodType[index] = 0;
    }
    
    if (playerNo == PLAYER_SABRE) {
        objData->capacityGamebits.small  = BIT_Sabre_Dino_Bag_S;
        objData->capacityGamebits.medium = BIT_Sabre_Dino_Bag_M;
        objData->capacityGamebits.large  = BIT_Sabre_Dino_Bag_L;
    } else if (playerNo == PLAYER_KRYSTAL) {
        objData->capacityGamebits.small  = BIT_Krystal_Dino_Bag_S;
        objData->capacityGamebits.medium = BIT_Krystal_Dino_Bag_M;
        objData->capacityGamebits.large  = BIT_Krystal_Dino_Bag_L;
    }
    
    objData->capacity = 0;
    objData->bagTimers = gDLL_29_Gplay->vtbl->func_19B8();
    main_set_bits(BIT_Dino_Foodbag_Place, TRUE);
    main_set_bits(BIT_Dino_Foodbag_Give, TRUE);
    objData->dllPutdown->vtbl->putdown_update_food_quantity_gamebits(objData->bagTimers, dino_foodbag_items);
    self->unkB0 |= 0x2000;
}

// offset: 0x1A4 | func: 1 | export: 1
void SideFoodbag_control(Object* self) {
    s32 dataIndex;
    GplayStruct14 *struct14;
    s32 uiValue;
    s32 uiGamebit;
    u16 index;
    u16 temp;
    SideFoodbag_Data* objData;

    objData = self->data;

    uiValue = gDLL_1_UI->vtbl->func_E2C(dino_foodbag_gamebitIDs, ARRAYCOUNT(dino_foodbag_gamebitIDs));
    uiGamebit = gDLL_1_UI->vtbl->func_F40();

    SideFoodbag_set_capacity(self);
    
    if (uiValue != NO_GAMEBIT) {
        for (index = 0; index < 30; index++){
            temp = objData->bagTimers->foodType[index];
            if (!temp){
                break;
            }

            dataIndex = objData->dllPutdown->vtbl->putdown_func_AFC(temp);
            if (uiValue == dino_foodbag_items[dataIndex].gamebitID) {
                switch (uiGamebit) {
                case BIT_Dino_Foodbag_Give:
                    break;
                case BIT_Dino_Foodbag_Place:
                    if (objData->dllPutdown->vtbl->putdown_place_food(self, temp, &objData->placedObjects, dino_foodbag_items)) {
                        SideFoodbag_func_594(self, uiValue);
                    }
                    break;
                }
                break;
            }
        }
    }
    
    temp = objData->dllPutdown->vtbl->putdown_func_47C(objData->bagTimers, dino_foodbag_items);
    if (temp) {
        SideFoodbag_func_530(self, temp);
    }
}

// offset: 0x38C | func: 2 | export: 2
void SideFoodbag_update(Object *self) { }

// offset: 0x398 | func: 3 | export: 3
void SideFoodbag_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x3B0 | func: 4 | export: 4
void SideFoodbag_free(Object* self, s32 arg1) {
    SideFoodbag_Data *objData = self->data;
    dll_unload(objData->dllPutdown);
}

// offset: 0x3F8 | func: 5 | export: 5
u32 SideFoodbag_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x408 | func: 6 | export: 6
u32 SideFoodbag_get_data_size(Object *self, u32 a1) {
    return sizeof(SideFoodbag_Data);
}

// offset: 0x41C | func: 7 | export: 7
s32 SideFoodbag_func_41C(Object* self) {
    SideFoodbag_Data *objData = self->data;
    if (objData->capacity != 0) {
        return 1;
    }
    return 0;
}

// offset: 0x440 | func: 8 | export: 8
void SideFoodbag_func_440(Object* self, Object* arg1, s32 arg2) {
    SideFoodbag_Data* objData = self->data;

    objData->dllPutdown->vtbl->putdown_get_nearest_placed_food_of_type(self, arg1, arg2, &objData->placedObjects);
}

// offset: 0x488 | func: 9 | export: 9
void SideFoodbag_func_488(Object* self, Object* arg1) {
    SideFoodbag_Data* objData = self->data;

    objData->dllPutdown->vtbl->putdown_destroy_placed_food(arg1, &objData->placedObjects);
}

// offset: 0x4DC | func: 10 | export: 10
void SideFoodbag_set_capacity(Object* self) {
    SideFoodbag_Data* objData = self->data;

    objData->capacity = objData->dllPutdown->vtbl->putdown_get_capacity(&objData->capacityGamebits);
}

// offset: 0x530 | func: 11 | export: 11
void SideFoodbag_func_530(Object* self, s32 arg1) {
    SideFoodbag_Data* objData = self->data;

    objData->dllPutdown->vtbl->putdown_func_5E8(arg1, objData->capacity, objData->bagTimers, dino_foodbag_items);
}

// offset: 0x594 | func: 12 | export: 12
void SideFoodbag_func_594(Object* self, s16 arg1) {
    SideFoodbag_Data* objData = self->data;

    objData->dllPutdown->vtbl->putdown_func_730(arg1, objData->bagTimers, dino_foodbag_items);
}

/*0x0*/ static const char str_0[] = "submenu %d\n";
/*0xC*/ static const char str_C[] = "add food %d\n";
/*0x1C*/ static const char str_1C[] = "";
