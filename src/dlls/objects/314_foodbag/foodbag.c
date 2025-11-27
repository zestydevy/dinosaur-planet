#include "common.h"

#include "dlls/objects/314_foodbag.h"

typedef struct {
    ObjSetup base;
} Foodbag_Setup;

typedef struct {
    DLL_Unknown *dllPutdown; //DLL #56, handling removing food items from bag
    s16 gamebitSmall;   //gamebitID for obtaining small food bag
    s16 gamebitMedium;  //gamebitID for obtaining medium food bag
    s16 gamebitLarge;   //gamebitID for obtaining large food bag
    u16 unkA;
    GplayStruct14* bagTimers;
    s8 *playerHealth;
    s8 *playerHealthMax;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s8 unk1C;
    s32 unk20[20];
    s16 unk70[20];
    f32 unk98;
} DLL314_Data; //9C

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
    {0,      0, 0, 0x000, NO_GAMEBIT,            NO_FOOD_OBJECT_ID}, 
    {2000,   1, 1, 0x004, BIT_Green_Apple_Count, OBJ_foodbagRedApple},
    {6000,   4, 1, 0x004, BIT_Red_Apple_Count,   OBJ_foodbagRedApple},
    {6000,   2, 1, 0x000, BIT_Brown_Apple_Count, OBJ_foodbagRedApple}, 
    {6000,   4, 2, 0x010, BIT_Fish_Count,        OBJ_foodbagNewFish}, 
    {6000,   8, 2, 0x000, BIT_Smoked_Fish_Count, OBJ_foodbagNewFish}, 
    {30000,  8, 1, 0x040, BIT_Dino_Egg_Count,    OBJ_foodbagNewMeat}, 
    {30000,  4, 1, 0x000, BIT_Moldy_Meat_Count,  OBJ_foodbagNewMeat}, 
    {30000,  1, 1, 0x200, BIT_Green_Bean_Count,  OBJ_foodbagGreenBea}, 
    {30000,  4, 1, 0x200, BIT_Red_Bean_Count,    OBJ_foodbagGreenBea}, 
    {30000, 1, 1, 0x000, BIT_Brown_Bean_Count,   OBJ_foodbagGreenBea}, 
    {30000, 2, 1, 0x200, BIT_Blue_Bean_Count,    OBJ_foodbagGreenBea}
};

/*0xA4*/ static s32 foodbag_gamebitIDs[] = {
    BIT_383, 
    BIT_384, 
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

s32 Foodbag_func_650(Object* self, s32 arg1);
void Foodbag_func_708(Object* self, s32 arg1);
void Foodbag_func_7E0(Object* self, s16 arg1);
void Foodbag_func_83C(Object* self);
static s32 Foodbag_eat_food(DLL314_Data* objData, s32 arg1);

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
    DLL314_Data* objData;

    objData = self->data;
    
    playerNo = gDLL_29_Gplay->vtbl->get_playerno();
    objData->dllPutdown = dll_load_deferred(0x38, 0xA);
    objData->unk1C = 0;

    for (index = 0; index < 20; index++){
        objData->unk20[index] = 0;
        objData->unk70[index] = 0;
    }
    
    objData->unk98 = -1.0f;
    
    if (playerNo == PLAYER_SABRE) {
        objData->gamebitSmall = BIT_Sabre_Foodbag_S;
        objData->gamebitMedium = BIT_Sabre_Foodbag_M;
        objData->gamebitLarge = BIT_Sabre_Foodbag_L;
    } else if (playerNo == PLAYER_KRYSTAL) {
        objData->gamebitSmall = BIT_Krystal_Foodbag_S;
        objData->gamebitMedium = BIT_Krystal_Foodbag_M;
        objData->gamebitLarge = BIT_Krystal_Foodbag_L;
    }
    
    objData->unkA = 0;
    objData->bagTimers = gDLL_29_Gplay->vtbl->func_1974();
    
    if (main_get_bits(BIT_383)) {
        objData->unk18 = 1;
    } else {
        objData->unk18 = 0;
    }
    
    playerStats = gDLL_29_Gplay->vtbl->get_player_stats();
    objData->playerHealth = &playerStats->health;
    objData->playerHealthMax = &playerStats->healthMax;
    
    if ((main_get_bits(BIT_384) == 0) && (main_get_bits(BIT_383) == 0)) {
        main_set_bits(BIT_385, 1);
        main_set_bits(BIT_386, 1);
        main_set_bits(BIT_387, 1);
        main_set_bits(BIT_384, 1);
    }
}

// offset: 0x22C | func: 1 | export: 1
void Foodbag_control(Object* self) {
    u16 index;
    s32 uiValue;
    s32 uiOtherGamebit;
    s32 *gamebitID;
    u16 temp;
    DLL314_Data* objData;
    FoodbagItem* food;

    objData = self->data;
    
    gDLL_1_UI->vtbl->func_70A0(0);
    uiValue = gDLL_1_UI->vtbl->func_E2C(foodbag_gamebitIDs, ARRAYCOUNT(foodbag_gamebitIDs));
    uiOtherGamebit = gDLL_1_UI->vtbl->func_F40();
    gDLL_1_UI->vtbl->func_70A0(1);
    Foodbag_func_83C(self);
    
    if (uiValue != NO_GAMEBIT) {
        if (uiValue == BIT_383) {
            Foodbag_func_650(self, 0);
        } else if (uiValue == BIT_384) {
            Foodbag_func_650(self, 1);
        } else {
            for (index = 0; index < 30; index++){
                temp = objData->bagTimers->unk78[index];
                if (!temp){
                    break;
                }
                
                food = &foodbag_items[((DLL_Unknown*)objData->dllPutdown)->vtbl->func[9].withOneArgS32(temp)];
                if (uiValue == food->gamebitID) {
                    gamebitID = &uiOtherGamebit;
                    switch (*gamebitID){
                    case BIT_387:
                        break;
                    case BIT_385:
                        if ((*objData->playerHealth < *objData->playerHealthMax) && food->healthRestored && Foodbag_eat_food(objData, temp)) {
                            Foodbag_func_7E0(self, uiValue);
                        }
                        break;
                    case BIT_386:
                        if (((DLL_Unknown*)objData->dllPutdown)->vtbl->func[0].withFourArgsS32((s32)self, temp, (s32)&objData->unk1C, (s32)&foodbag_items)) {
                            Foodbag_func_7E0(self, uiValue);
                        }
                        break;
                    }
                    break;
                }
            }


            
        }
    }
    
    temp = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[3].withTwoArgsU16((s32)objData->bagTimers, (s32)&foodbag_items);
    if (temp) {
        Foodbag_func_708(self, temp);
    }
}

// offset: 0x500 | func: 2 | export: 2
void Foodbag_update(Object *self) { }

// offset: 0x50C | func: 3 | export: 3
void Foodbag_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x524 | func: 4 | export: 4
void Foodbag_free(Object *self, s32 arg1) {
    DLL314_Data *objData = self->data;
    dll_unload(objData->dllPutdown);
}

// offset: 0x56C | func: 5 | export: 5
u32 Foodbag_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x57C | func: 6 | export: 6
u32 Foodbag_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL314_Data);
}

// offset: 0x590 | func: 7 | export: 7
int Foodbag_func_590(Object* self) {
    DLL314_Data *objData = self->data;
    if (objData->unkA) {
        return 1;
    }
    return 0;
}

// offset: 0x5B4 | func: 8 | export: 8
void Foodbag_func_5B4(Object* self, s32 arg1, s32 arg2) {
    DLL314_Data *objData = self->data;
    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[1].withFourArgs((s32)self, arg1, arg2, (s32)&objData->unk1C);
}

// offset: 0x5FC | func: 9 | export: 9
void Foodbag_func_5FC(Object* self, s32 arg1) {
    DLL314_Data *objData = self->data;
    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[2].withThreeArgs(arg1, (s32)&objData->unk1C, (s32)self);
}

// offset: 0x650 | func: 10 | export: 13
s32 Foodbag_func_650(Object* self, s32 arg1) {
    DLL314_Data* objData;
    s32 unk18;

    objData = self->data;
    
    unk18 = objData->unk18;
    if (arg1 == 0) {
        main_set_bits(BIT_383, 0);
        main_set_bits(BIT_384, 1);
        objData->unk18 = 0;
    } else {
        main_set_bits(BIT_383, 1);
        main_set_bits(BIT_384, 0);
        objData->unk18 = 1;
    }
    return unk18;
}

// offset: 0x708 | func: 11 | export: 11
void Foodbag_func_708(Object* self, s32 arg1) {
    DLL314_Data* objData = self->data;

    if (((objData->unk18 == 1) && (*objData->playerHealth < *objData->playerHealthMax)) || objData->unkA == 0) {
        Foodbag_eat_food(objData, arg1);
    } else if (((DLL_Unknown*)objData->dllPutdown)->vtbl->func[4].withFourArgsS32(arg1, objData->unkA, (s32)objData->bagTimers, (s32)&foodbag_items) == 0) {
        Foodbag_eat_food(objData, arg1);
    }
}

// offset: 0x7E0 | func: 12 | export: 12
void Foodbag_func_7E0(Object* self, s16 arg1) {
    DLL314_Data* objData = self->data;
    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[5].withFourArgs(arg1, (s32)objData->bagTimers, (s32)&foodbag_items, (s32)self);
}

// offset: 0x83C | func: 13 | export: 10
void Foodbag_func_83C(Object* self) {
    DLL314_Data* objData = self->data;
    objData->unkA = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[7].withTwoArgsS32((s32)&objData->gamebitSmall, (s32)self);
}

// offset: 0x890 | func: 14
s32 Foodbag_eat_food(DLL314_Data* objData, s32 arg1) {
    u8 healthRestored;
    s32 currentHealth;
    s8 *playerHealth;
    
    healthRestored = foodbag_items[((DLL_Unknown *) objData->dllPutdown)->vtbl->func[9].withOneArgS32(arg1)].healthRestored;
    
    if (healthRestored){
        playerHealth = objData->playerHealth;
        currentHealth = playerHealth[0];
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
            objData->unk98 = 120.0f;
        }
        gDLL_6_AMSFX->vtbl->play_sound(get_player(), SOUND_5EE_Eating_Food, MAX_VOLUME, 0, 0, 0, 0);
        return TRUE;
    }

    return FALSE;
}

// offset: 0xA0C | func: 15 | export: 14
u16 Foodbag_func_A0C(Object* self) {
    DLL314_Data *objData = self->data;
    return objData->unkA;
}

// offset: 0xA1C | func: 16 | export: 15
s32 Foodbag_func_A1C(Object* self) {
    DLL314_Data* objData;
    s8 index;

    objData = self->data;
    
    index = 0;
    while (objData->bagTimers->unk78[index]) {
        index++;
        if (index == objData->unkA) {
            return objData->unkA;
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
