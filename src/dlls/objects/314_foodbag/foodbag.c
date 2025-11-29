#include "common.h"

#include "dlls/engine/56_putdown.h"
#include "dlls/objects/314_foodbag.h"

/*0x0*/ static s16 food_anim_gamebitIDs[] = {
    BIT_Green_Apple_Count, 
    BIT_Red_Apple_Count, 
    BIT_Brown_Apple_Count, 
    BIT_Fish_Count, 
    BIT_Smoked_Fish_Count, 
    BIT_Dino_Egg_Count, 
    BIT_Moldy_Meat_Count, 
    BIT_Green_Bean_Count, 
    BIT_Red_Bean_Count, 
    BIT_Brown_Bean_Count, 
    BIT_Blue_Bean_Count
};

/*0x18*/ static s16 food_anim_objectIDs[] = {
    OBJ_appleAnim,
    OBJ_appleAnim,
    OBJ_appleAnim,
    OBJ_fishAnim,
    OBJ_fishAnim,
    OBJ_meatAnim,
    OBJ_meatAnim,
    OBJ_beanAnim,
    OBJ_beanAnim,
    OBJ_beanAnim
    //@bug? No corresponding objectID for BIT_Blue_Bean_Count
};

/*0x2C*/ static FoodbagItem foodbag_items[] = {
    {0,      0, 0,  0,               NO_GAMEBIT,             NO_FOOD_OBJECT_ID}, 
    {2000,   1, 1,  FOOD_TYPE(3),    BIT_Green_Apple_Count,  OBJ_foodbagRedApple},
    {6000,   4, 1,  FOOD_TYPE(3),    BIT_Red_Apple_Count,    OBJ_foodbagRedApple},
    {6000,   2, 1,  FOOD_GONE,       BIT_Brown_Apple_Count,  OBJ_foodbagRedApple}, 
    {6000,   4, 2,  FOOD_TYPE(5),    BIT_Fish_Count,         OBJ_foodbagNewFish}, 
    {6000,   8, 2,  FOOD_GONE,       BIT_Smoked_Fish_Count,  OBJ_foodbagNewFish}, 
    {30000,  8, 1,  FOOD_TYPE(7),    BIT_Dino_Egg_Count,     OBJ_foodbagNewMeat}, 
    {30000,  4, 1,  FOOD_GONE,       BIT_Moldy_Meat_Count,   OBJ_foodbagNewMeat}, 
    {30000,  1, 1,  FOOD_TYPE(10),   BIT_Green_Bean_Count,   OBJ_foodbagGreenBea}, 
    {30000,  4, 1,  FOOD_TYPE(10),   BIT_Red_Bean_Count,     OBJ_foodbagGreenBea}, 
    {30000, 1, 1,   FOOD_GONE,       BIT_Brown_Bean_Count,   OBJ_foodbagGreenBea}, 
    {30000, 2, 1,   FOOD_TYPE(10),   BIT_Blue_Bean_Count,    OBJ_foodbagGreenBea}
};

/*0xA4*/ static s32 foodbag_cmdmenu_gamebitIDs[] = {
    BIT_Foodbag_Setting_Eat_First, 
    BIT_Foodbag_Setting_Eat_Later, 
    BIT_Green_Apple_Count, 
    BIT_Red_Apple_Count, 
    BIT_Brown_Apple_Count, 
    BIT_Fish_Count, 
    BIT_Smoked_Fish_Count, 
    BIT_Dino_Egg_Count, 
    BIT_Moldy_Meat_Count, 
    BIT_Green_Bean_Count, 
    BIT_Red_Bean_Count, 
    BIT_Brown_Bean_Count, 
    BIT_Blue_Bean_Count
};

int Foodbag_set_eat_config(Object* self, s32 eatFirst);
void Foodbag_collect_food(Object* self, s32 arg1);
void Foodbag_delete_food_by_gamebit(Object* self, s16 arg1);
void Foodbag_set_capacity(Object* self);
static s32 Foodbag_eat_food(Foodbag_Data* objData, s32 arg1);

// offset: 0x0 | ctor
void Foodbag_ctor(void *dll) { }

// offset: 0xC | dtor
void Foodbag_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Foodbag_setup(Object *self, Foodbag_Setup *objSetup, s32 arg2) {
    s32 pad[2];
    s32 playerNo;
    PlayerStats* playerStats;
    s32 index;
    Foodbag_Data* objData;

    objData = self->data;
    
    playerNo = gDLL_29_Gplay->vtbl->get_playerno();
    objData->dllPutdown = dll_load_deferred(56, 10);
    objData->placedObjects.nextIndex = 0;

    for (index = 0; index < 20; index++){
        objData->placedObjects.objects[index] = NULL;
        objData->placedObjects.foodType[index] = 0;
    }
    
    objData->playerEatTimer = -1.0f;
    
    if (playerNo == PLAYER_SABRE) {
        objData->capacityGamebits.small  = BIT_Sabre_Foodbag_S;
        objData->capacityGamebits.medium = BIT_Sabre_Foodbag_M;
        objData->capacityGamebits.large  = BIT_Sabre_Foodbag_L;
    } else if (playerNo == PLAYER_KRYSTAL) {
        objData->capacityGamebits.small  = BIT_Krystal_Foodbag_S;
        objData->capacityGamebits.medium = BIT_Krystal_Foodbag_M;
        objData->capacityGamebits.large  = BIT_Krystal_Foodbag_L;
    }
    
    objData->capacity = 0;
    objData->bagSlots = gDLL_29_Gplay->vtbl->func_1974();
    
    if (main_get_bits(BIT_Foodbag_Setting_Eat_First)) {
        objData->eatFirst = TRUE;
    } else {
        objData->eatFirst = FALSE;
    }
    
    playerStats = gDLL_29_Gplay->vtbl->get_player_stats();
    objData->playerHealth = &playerStats->health;
    objData->playerHealthMax = &playerStats->healthMax;
    
    //Set initial foodbag config, if none set
    if (main_get_bits(BIT_Foodbag_Setting_Eat_Later) == FALSE && 
        main_get_bits(BIT_Foodbag_Setting_Eat_First) == FALSE) {

        //Make sure eat/place/give options are available
        main_set_bits(BIT_Foodbag_Eat, TRUE);
        main_set_bits(BIT_Foodbag_Place, TRUE);
        main_set_bits(BIT_Foodbag_Give, TRUE);
        
        //Default to "Eat Later" config
        main_set_bits(BIT_Foodbag_Setting_Eat_Later, TRUE);
    }
}

// offset: 0x22C | func: 1 | export: 1
void Foodbag_control(Object* self) {
    u16 index;
    s32 uiGamebit;
    s32 uiSubmenuGamebit;
    s32 *submenuGamebit;
    u16 foodType;
    Foodbag_Data* objData;
    FoodbagItem* food;

    objData = self->data;
    
    gDLL_1_UI->vtbl->func_70A0(0);
    uiGamebit = gDLL_1_UI->vtbl->func_E2C(foodbag_cmdmenu_gamebitIDs, ARRAYCOUNT(foodbag_cmdmenu_gamebitIDs));
    uiSubmenuGamebit = gDLL_1_UI->vtbl->func_F40();
    gDLL_1_UI->vtbl->func_70A0(1);
    Foodbag_set_capacity(self);
    
    //Check if a relevant item was selected in the cmdmenu
    if (uiGamebit != NO_GAMEBIT) {
        if (uiGamebit == BIT_Foodbag_Setting_Eat_First) {
            Foodbag_set_eat_config(self, FALSE);
        } else if (uiGamebit == BIT_Foodbag_Setting_Eat_Later) {
            Foodbag_set_eat_config(self, TRUE);
        } else {
            for (index = 0; index < 30; index++){
                foodType = objData->bagSlots->foodType[index];
                if (!foodType){
                    break;
                }
                food = &foodbag_items[objData->dllPutdown->vtbl->get_foodID_from_foodType(foodType)];

                //If a food item was selected
                if (uiGamebit == food->gamebitID) {
                    submenuGamebit = &uiSubmenuGamebit;
                    switch (*submenuGamebit){
                    case BIT_Foodbag_Give:
                        break;
                    case BIT_Foodbag_Eat:
                        if ((*objData->playerHealth < *objData->playerHealthMax) && 
                                food->healthRestored && 
                                Foodbag_eat_food(objData, foodType)) {
                            Foodbag_delete_food_by_gamebit(self, uiGamebit);
                        }
                        break;
                    case BIT_Foodbag_Place:
                        if (objData->dllPutdown->vtbl->place_food(self, foodType, &objData->placedObjects, foodbag_items)) {
                            Foodbag_delete_food_by_gamebit(self, uiGamebit);
                        }
                        break;
                    }
                    break;
                }
            }
        }
    }
    
    foodType = objData->dllPutdown->vtbl->tick_food_lifetimes(objData->bagSlots, foodbag_items);
    if (foodType) {
        Foodbag_collect_food(self, foodType);
    }
}

// offset: 0x500 | func: 2 | export: 2
void Foodbag_update(Object *self) { }

// offset: 0x50C | func: 3 | export: 3
void Foodbag_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x524 | func: 4 | export: 4
void Foodbag_free(Object *self, s32 arg1) {
    Foodbag_Data *objData = self->data;
    dll_unload(objData->dllPutdown);
}

// offset: 0x56C | func: 5 | export: 5
u32 Foodbag_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x57C | func: 6 | export: 6
u32 Foodbag_get_data_size(Object *self, u32 a1) {
    return sizeof(Foodbag_Data);
}

// offset: 0x590 | func: 7 | export: 7
int Foodbag_is_obtained(Object* self) {
    Foodbag_Data *objData = self->data;
    if (objData->capacity) {
        return TRUE;
    }
    return FALSE;
}

// offset: 0x5B4 | func: 8 | export: 8
Object* Foodbag_get_nearest_placed_food_of_type(Object* self, Object* target, s32 foodType) {
    Foodbag_Data *objData = self->data;

    return objData->dllPutdown->vtbl->get_nearest_placed_food_of_type(self, target, foodType, &objData->placedObjects);
}

// offset: 0x5FC | func: 9 | export: 9
int Foodbag_destroy_placed_food(Object* self, Object* foodObject) {
    Foodbag_Data *objData = self->data;

    return objData->dllPutdown->vtbl->destroy_placed_food(foodObject, &objData->placedObjects);
}

// offset: 0x650 | func: 10 | export: 13
/**
  * Switches the "Eat First" behaviour on/off, and returns the previous state of the setting
  */
int Foodbag_set_eat_config(Object* self, s32 eatFirst) {
    Foodbag_Data* objData;
    s32 previousSetting;

    objData = self->data;
    
    previousSetting = objData->eatFirst;
    if (eatFirst == FALSE) {
        main_set_bits(BIT_Foodbag_Setting_Eat_First, FALSE);
        main_set_bits(BIT_Foodbag_Setting_Eat_Later, TRUE);
        objData->eatFirst = FALSE;
    } else {
        main_set_bits(BIT_Foodbag_Setting_Eat_First, TRUE);
        main_set_bits(BIT_Foodbag_Setting_Eat_Later, FALSE);
        objData->eatFirst = TRUE;
    }

    return previousSetting;
}

// offset: 0x708 | func: 11 | export: 11
/**
  * If "Eat First" is enabled and the player's not at max health, eats the food instead of storing it.
  * Otherwise, attempts to store the food - if the bag doesn't have room, eats the food instead!
  */
void Foodbag_collect_food(Object* self, s32 foodType) {
    Foodbag_Data* objData = self->data;

    if ((objData->eatFirst == TRUE && (*objData->playerHealth < *objData->playerHealthMax)) 
        || objData->capacity == 0) {
        Foodbag_eat_food(objData, foodType);
    } else if (objData->dllPutdown->vtbl->add_food(foodType, objData->capacity, objData->bagSlots, foodbag_items) == FALSE) {
        Foodbag_eat_food(objData, foodType);
    }
}

// offset: 0x7E0 | func: 12 | export: 12
/**
  * Deletes the first instance of a food item from the bag using its inventory gamebit (also decrements the gamebit's quantity)
  */
void Foodbag_delete_food_by_gamebit(Object* self, s16 gamebit) {
    Foodbag_Data* objData = self->data;

    objData->dllPutdown->vtbl->delete_food_by_gamebit(gamebit, objData->bagSlots, foodbag_items);
}

// offset: 0x83C | func: 13 | export: 10
void Foodbag_set_capacity(Object* self) {
    Foodbag_Data* objData = self->data;

    objData->capacity = objData->dllPutdown->vtbl->get_capacity(&objData->capacityGamebits);
}

// offset: 0x890 | func: 14
s32 Foodbag_eat_food(Foodbag_Data* objData, s32 foodType) {
    u8 healthRestored;
    s32 currentHealth;
    s8 *playerHealth;
    
    healthRestored = foodbag_items[objData->dllPutdown->vtbl->get_foodID_from_foodType(foodType)].healthRestored;
    
    if (healthRestored){
        playerHealth = objData->playerHealth;
        currentHealth = *playerHealth;
        if (healthRestored + currentHealth > *objData->playerHealthMax) { 
            if (objData->playerHealth && objData->playerHealth && objData->playerHealth) {} //@fake?
            
            if (*objData->playerHealthMax != currentHealth) {
                gDLL_6_AMSFX->vtbl->play_sound(get_player(), SOUND_5EE_Eating_Food, MAX_VOLUME, 0, 0, 0, 0);
            }
            *objData->playerHealth = *objData->playerHealthMax;
            return TRUE;
        }
        
        *objData->playerHealth = healthRestored + currentHealth;
        if (*objData->playerHealth == *objData->playerHealthMax){
            objData->playerEatTimer = 120.0f;
        }
        gDLL_6_AMSFX->vtbl->play_sound(get_player(), SOUND_5EE_Eating_Food, MAX_VOLUME, 0, 0, 0, 0);
        return TRUE;
    }

    return FALSE;
}

// offset: 0xA0C | func: 15 | export: 14
u16 Foodbag_get_capacity(Object* self) {
    Foodbag_Data *objData = self->data;
    return objData->capacity;
}

// offset: 0xA1C | func: 16 | export: 15
s32 Foodbag_count_slots_occupied(Object* self) {
    Foodbag_Data* objData;
    s8 index;

    objData = self->data;
    
    index = 0;
    while (objData->bagSlots->foodType[index]) {
        index++;
        if (index == objData->capacity) {
            return objData->capacity;
        }
    }
    
    return index;
}

// offset: 0xA74 | func: 17 | export: 16
s16 Foodbag_get_anim_objectID_from_food_gamebit(s16 foodGamebit) {
    s32 index;

    for (index = 0; index < 10 && foodGamebit != food_anim_gamebitIDs[index]; index++){}
    
    if (index == 10) {
        index = 0;
    }

    STUBBED_PRINTF(" Food Type Found %i : Obj %i ");
    
    return food_anim_objectIDs[index];
}
