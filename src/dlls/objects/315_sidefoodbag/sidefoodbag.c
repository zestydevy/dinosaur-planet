#include "common.h"

#include "dlls/objects/314_foodbag.h"

typedef struct {
    ObjSetup base;
} SideFoodbag_Setup;

typedef struct {
    DLL_Unknown *dllPutdown; //DLL #56, handling removing food items from bag
    FoodbagGamebits gamebits;
    u16 capacity;
    GplayStruct14* bagTimers;
    FoodbagStructUnk unk10;
} SideFoodbag_Data; //0x8C

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

void SideFoodbag_func_4DC(Object* self);
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
    objData->dllPutdown = (DLL_Unknown*)dll_load_deferred(56, 10);
    objData->unk10.nextIndex = 0;
    
    for (index = 0; index < 20; index++){
        objData->unk10.placedObjects[index] = 0;
        objData->unk10.foodType[index] = 0;
    }
    
    if (playerNo == PLAYER_SABRE) {
        objData->gamebits.small  = BIT_Sabre_Dino_Bag_S;
        objData->gamebits.medium = BIT_Sabre_Dino_Bag_M;
        objData->gamebits.large  = BIT_Sabre_Dino_Bag_L;
    } else if (playerNo == PLAYER_KRYSTAL) {
        objData->gamebits.small  = BIT_Krystal_Dino_Bag_S;
        objData->gamebits.medium = BIT_Krystal_Dino_Bag_M;
        objData->gamebits.large  = BIT_Krystal_Dino_Bag_L;
    }
    
    objData->capacity = 0;
    objData->bagTimers = gDLL_29_Gplay->vtbl->func_19B8();
    main_set_bits(BIT_200, 1);
    main_set_bits(BIT_22C, 1);
    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[8].withTwoArgs((s32)objData->bagTimers, (s32)&dino_foodbag_items);
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

    SideFoodbag_func_4DC(self);
    
    if (uiValue != NO_GAMEBIT) {
        for (index = 0; index < 30; index++){
            temp = objData->bagTimers->unk78[index];
            if (!temp){
                break;
            }

            dataIndex = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[9].withOneArgS32(temp);
            if (uiValue == dino_foodbag_items[dataIndex].gamebitID) {
                switch (uiGamebit) {
                case BIT_22C:
                    break;
                case BIT_200:
                    if (((DLL_Unknown*)objData->dllPutdown)->vtbl->func[0].withFourArgsS32((s32)self, temp, (s32)&objData->unk10, (s32)&dino_foodbag_items)) {
                        SideFoodbag_func_594(self, uiValue);
                    }
                    break;
                }
                break;
            }
        }
    }
    
    temp = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[3].withTwoArgsU16((s32)objData->bagTimers, (s32)&dino_foodbag_items);
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
void SideFoodbag_func_440(Object* self, s32 arg1, s32 arg2) {
    SideFoodbag_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[1].withFourArgs((s32)self, arg1, arg2, (s32)&objData->unk10);
}

// offset: 0x488 | func: 9 | export: 9
void SideFoodbag_func_488(Object* self, s32 arg1) {
    SideFoodbag_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[2].withThreeArgs(arg1, (s32)&objData->unk10, (s32)self);
}

// offset: 0x4DC | func: 10 | export: 10
void SideFoodbag_func_4DC(Object* self) {
    SideFoodbag_Data* objData = self->data;

    objData->capacity = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[7].withOneArgS32((s32)&objData->gamebits);
}

// offset: 0x530 | func: 11 | export: 11
void SideFoodbag_func_530(Object* self, s32 arg1) {
    SideFoodbag_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[4].withFourArgs(arg1, objData->capacity, (s32)objData->bagTimers, (s32)&dino_foodbag_items);
}

// offset: 0x594 | func: 12 | export: 12
void SideFoodbag_func_594(Object* self, s16 arg1) {
    SideFoodbag_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[5].withThreeArgs(arg1, (s32)objData->bagTimers, (s32)&dino_foodbag_items);
}

/*0x0*/ static const char str_0[] = "submenu %d\n";
/*0xC*/ static const char str_C[] = "add food %d\n";
/*0x1C*/ static const char str_1C[] = "";
