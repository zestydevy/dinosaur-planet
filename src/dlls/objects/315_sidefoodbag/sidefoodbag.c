#include "common.h"
#include "dlls/engine/29_gplay.h"
#include "game/gamebits.h"
#include "macros.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
} DLL315_Setup;

typedef struct {
    DLL_Unknown *dllPutdown; //DLL #56, handling removing food items from bag
    s16 gamebitSmall;   //gamebitID for obtaining small dino food bag
    s16 gamebitMedium;  //gamebitID for obtaining medium dino food bag
    s16 gamebitLarge;   //gamebitID for obtaining large dino food bag
    u16 unkA;
    GplayStruct14* bagTimers;
    s8 unk10;
    s32 unk14[20];
    s16 unk64[20];
} DLL315_Data; //0x8C

typedef struct {
    s16 expiry;
    s16 unk2;
    s16 unk4;
    s16 gamebit; //gamebitID of inventory item
    s16 unk8;
} FoodbagItem;

#define BLANK_FOODBAG_ITEM {0, 0, 0, 0, 0}

/*0x0*/ static FoodbagItem dino_foodbag_items[] = {
    {0,     0,          0,  NO_GAMEBIT,                     0xffff}, 
    {2000, 0x0201, 0x0004,  BIT_Dino_Bag_Blue_Mushrooms,    0x0400},
    {6000, 0x0201, 0x0004,  BIT_Dino_Bag_Red_Mushrooms,     0x0400},
    {6000, 0x0101, 0x0000,  BIT_Dino_Bag_Old_Mushrooms,     0x0400},
    {6000, 0x0201, 0x0020,  BIT_Dino_Bag_Blue_Grubs,        0x0401},
    {6000, 0x0201, 0x0020,  BIT_Dino_Bag_Red_Grubs,         0x0401},
    {30000, 0x0101, 0x0000, BIT_Dino_Bag_Old_Grubs,         0x0401},
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

void sidefoodbag_func_4DC(Object* self);
void sidefoodbag_func_530(Object* self, s32 arg1);
void sidefoodbag_func_594(Object* self, s16 arg1);

// offset: 0x0 | ctor
void sidefoodbag_ctor(void *dll) { }

// offset: 0xC | dtor
void sidefoodbag_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void sidefoodbag_setup(Object* self, DLL315_Setup *objSetup, s32 arg2) {
    s32 pad[2];
    s32 playerNo;
    DLL315_Data* objData;
    s32 index;

    objData = self->data;
    playerNo = gDLL_29_Gplay->vtbl->get_playerno();
    objData->dllPutdown = (DLL_Unknown*)dll_load_deferred(56, 10);
    objData->unk10 = 0;
    
    for (index = 0; index < 20; index++){
        objData->unk14[index] = 0;
        objData->unk64[index] = 0;
    }
    
    if (playerNo == PLAYER_SABRE) {
        objData->gamebitSmall = BIT_Sabre_Dino_Bag_S;
        objData->gamebitMedium = BIT_Sabre_Dino_Bag_M;
        objData->gamebitLarge = BIT_Sabre_Dino_Bag_L;
    } else if (playerNo == PLAYER_KRYSTAL) {
        objData->gamebitSmall = BIT_Krystal_Dino_Bag_S;
        objData->gamebitMedium = BIT_Krystal_Dino_Bag_M;
        objData->gamebitLarge = BIT_Krystal_Dino_Bag_L;
    }
    
    objData->unkA = 0;
    objData->bagTimers = gDLL_29_Gplay->vtbl->func_19B8();
    main_set_bits(BIT_200, 1);
    main_set_bits(BIT_22C, 1);
    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[8].withTwoArgs((s32)objData->bagTimers, (s32)&dino_foodbag_items);
    self->unkB0 |= 0x2000;
}

// offset: 0x1A4 | func: 1 | export: 1
void sidefoodbag_control(Object* self) {
    s32 dataIndex;
    GplayStruct14 *struct14;
    s32 uiValue;
    s32 uiGamebit;
    u16 index;
    u16 temp;
    DLL315_Data* objData;

    objData = self->data;

    uiValue = gDLL_1_UI->vtbl->func_E2C(dino_foodbag_gamebitIDs, ARRAYCOUNT(dino_foodbag_gamebitIDs));
    uiGamebit = gDLL_1_UI->vtbl->func_F40();

    sidefoodbag_func_4DC(self);
    
    if (uiValue != -1) {
        for (index = 0; index < 30; index++){
            temp = objData->bagTimers->unk78[index];
            if (!temp){
                break;
            }

            dataIndex = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[9].withOneArgS32(temp);
            if (uiValue == dino_foodbag_items[dataIndex].gamebit) {
                switch (uiGamebit) {
                case BIT_22C:
                    break;
                case BIT_200:
                    if (((DLL_Unknown*)objData->dllPutdown)->vtbl->func[0].withFourArgsS32((s32)self, temp, (s32)&objData->unk10, (s32)&dino_foodbag_items)) {
                        sidefoodbag_func_594(self, uiValue);
                    }
                    break;
                }
                break;
            }
        }
    }
    
    temp = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[3].withTwoArgsU16((s32)objData->bagTimers, (s32)&dino_foodbag_items);
    if (temp) {
        sidefoodbag_func_530(self, temp);
    }
}

// offset: 0x38C | func: 2 | export: 2
void sidefoodbag_update(Object *self) { }

// offset: 0x398 | func: 3 | export: 3
void sidefoodbag_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x3B0 | func: 4 | export: 4
void sidefoodbag_free(Object* self, s32 arg1) {
    DLL315_Data *objData = self->data;
    dll_unload(objData->dllPutdown);
}

// offset: 0x3F8 | func: 5 | export: 5
u32 sidefoodbag_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x408 | func: 6 | export: 6
u32 sidefoodbag_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL315_Data);
}

// offset: 0x41C | func: 7 | export: 7
s32 sidefoodbag_func_41C(Object* self) {
    DLL315_Data *objData = self->data;
    if (objData->unkA != 0) {
        return 1;
    }
    return 0;
}

// offset: 0x440 | func: 8 | export: 8
void sidefoodbag_func_440(Object* self, s32 arg1, s32 arg2) {
    DLL315_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[1].withFourArgs((s32)self, arg1, arg2, (s32)&objData->unk10);
}

// offset: 0x488 | func: 9 | export: 9
void sidefoodbag_func_488(Object* self, s32 arg1) {
    DLL315_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[2].withThreeArgs(arg1, (s32)&objData->unk10, (s32)self);
}

// offset: 0x4DC | func: 10 | export: 10
void sidefoodbag_func_4DC(Object* self) {
    DLL315_Data* objData = self->data;

    objData->unkA = ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[7].withTwoArgsS32((s32)&objData->gamebitSmall, (s32)self);
}

// offset: 0x530 | func: 11 | export: 11
void sidefoodbag_func_530(Object* self, s32 arg1) {
    DLL315_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[4].withFourArgs(arg1, objData->unkA, (s32)objData->bagTimers, (s32)&dino_foodbag_items);
}

// offset: 0x594 | func: 12 | export: 12
void sidefoodbag_func_594(Object* self, s16 arg1) {
    DLL315_Data* objData = self->data;

    ((DLL_Unknown*)objData->dllPutdown)->vtbl->func[5].withThreeArgs(arg1, (s32)objData->bagTimers, (s32)&dino_foodbag_items);
}

/*0x0*/ static const char str_0[] = "submenu %d\n";
/*0xC*/ static const char str_C[] = "add food %d\n";
/*0x1C*/ static const char str_1C[] = "";
