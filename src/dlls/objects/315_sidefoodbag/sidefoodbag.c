#include "common.h"

#include "dlls/objects/315_sidefoodbag.h"
#include "macros.h"

/*0x0*/ static FoodbagItem dino_foodbag_items[] = {
    {0,     0, 0, 0,            NO_GAMEBIT,                   NO_FOOD_OBJECT_ID}, 
    {2000,  2, 1, FOOD_TYPE(3), BIT_Dino_Bag_Blue_Mushrooms,  OBJ_foodbagBlueMush},
    {6000,  2, 1, FOOD_TYPE(3), BIT_Dino_Bag_Red_Mushrooms,   OBJ_foodbagBlueMush},
    {6000,  1, 1, FOOD_GONE,    BIT_Dino_Bag_Old_Mushrooms,   OBJ_foodbagBlueMush},
    {6000,  2, 1, FOOD_TYPE(6), BIT_Dino_Bag_Blue_Grubs,      OBJ_foodbagBlueGrub},
    {6000,  2, 1, FOOD_TYPE(6), BIT_Dino_Bag_Red_Grubs,       OBJ_foodbagBlueGrub},
    {30000, 1, 1, FOOD_GONE,    BIT_Dino_Bag_Old_Grubs,       OBJ_foodbagBlueGrub},
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM,
    BLANK_FOODBAG_ITEM
};
/*0x78*/ static s32 dino_foodbag_cmdmenu_gamebitIDs[] = {
    BIT_Dino_Bag_Blue_Mushrooms,
    BIT_Dino_Bag_Red_Mushrooms, 
    BIT_Dino_Bag_Old_Mushrooms, 
    BIT_Dino_Bag_Blue_Grubs, 
    BIT_Dino_Bag_Red_Grubs, 
    BIT_Dino_Bag_Old_Grubs
};

void SideFoodbag_set_capacity(Object* self);
int SideFoodbag_collect_food(Object* self, s32 arg1);
void SideFoodbag_delete_food_by_gamebit(Object* self, s16 arg1);

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
    objData->bagSlots = gDLL_29_Gplay->vtbl->func_19B8();
    main_set_bits(BIT_Dino_Foodbag_Place, TRUE);
    main_set_bits(BIT_Dino_Foodbag_Give, TRUE);
    objData->dllPutdown->vtbl->putdown_update_food_quantity_gamebits(objData->bagSlots, dino_foodbag_items);
    self->unkB0 |= 0x2000;
}

// offset: 0x1A4 | func: 1 | export: 1
void SideFoodbag_control(Object* self) {
    s32 dataIndex;
    FoodbagContents *struct14;
    s32 uiGamebit;
    s32 uiSubmenuGamebit;
    u16 index;
    u16 foodType;
    SideFoodbag_Data* objData;

    objData = self->data;

    uiGamebit = gDLL_1_UI->vtbl->func_E2C(dino_foodbag_cmdmenu_gamebitIDs, ARRAYCOUNT(dino_foodbag_cmdmenu_gamebitIDs));
    uiSubmenuGamebit = gDLL_1_UI->vtbl->func_F40();

    STUBBED_PRINTF("submenu %d\n");

    SideFoodbag_set_capacity(self);

    //Check if a relevant food item was selected in the cmdmenu
    if (uiGamebit != NO_GAMEBIT) {
        for (index = 0; index < 30; index++){
            foodType = objData->bagSlots->foodType[index];
            if (!foodType){
                break;
            }

            dataIndex = objData->dllPutdown->vtbl->putdown_get_foodID_from_foodType(foodType);
            if (uiGamebit == dino_foodbag_items[dataIndex].gamebitID) {
                switch (uiSubmenuGamebit) {
                case BIT_Dino_Foodbag_Give:
                    break;
                case BIT_Dino_Foodbag_Place:
                    if (objData->dllPutdown->vtbl->putdown_place_food(self, foodType, &objData->placedObjects, dino_foodbag_items)) {
                        SideFoodbag_delete_food_by_gamebit(self, uiGamebit);
                    }
                    break;
                }
                break;
            }
        }
    }
    
    foodType = objData->dllPutdown->vtbl->putdown_tick_food_lifetimes(objData->bagSlots, dino_foodbag_items);
    if (foodType) {
        SideFoodbag_collect_food(self, foodType);
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
int SideFoodbag_is_obtained(Object* self) {
    SideFoodbag_Data *objData = self->data;
    if (objData->capacity) {
        return TRUE;
    }
    return FALSE;
}

// offset: 0x440 | func: 8 | export: 8
Object* SideFoodbag_get_nearest_placed_food_of_type(Object* self, Object* target, s32 foodType) {
    SideFoodbag_Data* objData = self->data;

    return objData->dllPutdown->vtbl->putdown_get_nearest_placed_food_of_type(self, target, foodType, &objData->placedObjects);
}

// offset: 0x488 | func: 9 | export: 9
int SideFoodbag_destroy_placed_food(Object* self, Object* foodObject) {
    SideFoodbag_Data* objData = self->data;

    return objData->dllPutdown->vtbl->putdown_destroy_placed_food(foodObject, &objData->placedObjects);
}

// offset: 0x4DC | func: 10 | export: 10
void SideFoodbag_set_capacity(Object* self) {
    SideFoodbag_Data* objData = self->data;

    objData->capacity = objData->dllPutdown->vtbl->putdown_get_capacity(&objData->capacityGamebits);
}

// offset: 0x530 | func: 11 | export: 11
int SideFoodbag_collect_food(Object* self, s32 foodType) {
    SideFoodbag_Data* objData = self->data;

    STUBBED_PRINTF("add food %d\n");

    return objData->dllPutdown->vtbl->putdown_add_food(foodType, objData->capacity, objData->bagSlots, dino_foodbag_items);
}

// offset: 0x594 | func: 12 | export: 12
/**
  * Deletes the first instance of a food item from the bag using its inventory gamebit (also decrements the gamebit's quantity)
  */
void SideFoodbag_delete_food_by_gamebit(Object* self, s16 foodGamebit) {
    SideFoodbag_Data* objData = self->data;

    STUBBED_PRINTF("");

    objData->dllPutdown->vtbl->putdown_delete_food_by_gamebit(foodGamebit, objData->bagSlots, dino_foodbag_items);
}
