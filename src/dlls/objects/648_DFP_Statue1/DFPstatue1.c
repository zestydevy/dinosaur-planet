#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 statueIdx;       //Which of the 4 statues this is (see note below)
    u16 unk1A;
    u16 unk1C;
    s16 gamebitUnused;
    s16 gamebitMove;    //The statue moves when this gamebit is set (by hitting the Projectile Switch underneath)
} DFP_Statue1_Setup;

typedef struct {
    s16 gamebitUnused;
    s16 gamebitMove;    //The statue moves when this gamebit is set (by hitting the Projectile Switch underneath)
    u8 hasMoved;        //Boolean: whether the statue is currently leaning over
    u8 statueIdx;       //Which of the 4 statues this is (see note below)
} DFP_Statue1_Data;

/* 
    Note about statue indices:

    The statues' unused internal indices (objData->statueIdx) are arranged like this:
    Statue 0 is the one just to your left as you walk into the puzzle room 
    (entering from the SpellStone Holder room's connecting corridor), 
    and then statues 1, 2, 3 proceed in clockwise order (viewing the room from above).

    Confusingly, the order of the statues' gamebits is handled differently!
    BIT_672 is for the statue just to the left of the perch switch (viewed from the 
    centre of the room), and then gamebits 673, 674, 675 are for the other statues, 
    proceeding in anticlockwise order around the room (viewed from above).
    The DFP_RotatePuzzle DLL's code uses this gamebit ordering for the statues.
*/

typedef enum {
    DFP_Statue1_ANIMCMD_1_Start_Blowing_at_Flames = 1,
    DFP_Statue1_ANIMCMD_2_Stop_Blowing_at_Flames = 2
} DFP_Statue1_AnimMessages;

/*0x0*/ static s16 dNumStatuesLeaning = 0;
/*0x4*/ static u8 dResetAllStatues = FALSE;

static void DFP_Statue1_tick(Object* self);
static int DFP_Statue1_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DFP_Statue1_ctor(void* dll) { }

// offset: 0xC | dtor
void DFP_Statue1_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_Statue1_obj_Setup(Object* self, DFP_Statue1_Setup* objSetup, s32 reset) {
    DFP_Statue1_Data* objData;

    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DFP_Statue1_animCallback;
    
    objData = self->data;
    objData->statueIdx = objSetup->statueIdx;
    objData->gamebitUnused = objSetup->gamebitUnused;
    objData->gamebitMove = objSetup->gamebitMove;

    if (mainGetBits(objData->gamebitMove)) {
        objData->hasMoved = TRUE;
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
}

// offset: 0xB4 | func: 1 | export: 1
void DFP_Statue1_obj_Control(Object* self) {
    DFP_Statue1_tick(self);
}

// offset: 0xF0 | func: 2 | export: 2
void DFP_Statue1_obj_Update(Object* self) {
    if (dNumStatuesLeaning == 4) {
        dNumStatuesLeaning = 0;
    }
}

// offset: 0x124 | func: 3 | export: 3
void DFP_Statue1_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x13C | func: 4 | export: 4
void DFP_Statue1_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x14C | func: 5 | export: 5
u32 DFP_Statue1_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x15C | func: 6 | export: 6
u32 DFP_Statue1_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFP_Statue1_Data);
}

// offset: 0x170 | func: 7
void DFP_Statue1_tick(Object* self) {
    DFP_Statue1_Data* objData = self->data;
    s16 gamebitMoveValue = mainGetBits(objData->gamebitMove);
    
    //The statue leans forward and blows at the flames when its gamebit is set
    if ((objData->hasMoved == FALSE) && (gamebitMoveValue != FALSE) && mainGetBits(BIT_DFP_RotatePuzzle_Started)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objData->hasMoved = TRUE;
    }
    
    //When all 4 statues are leaning, reset them back to their original position
    if (dResetAllStatues && objData->hasMoved && mainGetBits(BIT_DFP_RotatePuzzle_Started)) {
        mainSetBits(objData->gamebitMove, FALSE);
        gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        objData->hasMoved = FALSE;

        dNumStatuesLeaning--;
        if (dNumStatuesLeaning == 0) {
            dResetAllStatues = FALSE;
        }
    }
}

// offset: 0x2C0 | func: 8
int DFP_Statue1_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {  
    DFP_Statue1_Data* objData;
    s32 i;

    objData = self->data;
    animData->unk7A = -1;
    animData->unk62 = 0;

    //Set a gamebit while the statue is blowing at the flames (indexed 5 gamebits after the gamebit that moves the statue)
    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case DFP_Statue1_ANIMCMD_1_Start_Blowing_at_Flames:
            mainSetBits(objData->gamebitMove + 5, TRUE);
            break;
        case DFP_Statue1_ANIMCMD_2_Stop_Blowing_at_Flames:
            mainSetBits(objData->gamebitMove + 5, FALSE);
            dNumStatuesLeaning++;
            if (dNumStatuesLeaning == 4) {
                dResetAllStatues = TRUE;
            }
            break;
        }

        animData->messages[i] = 0;
    }
    
    return 0;
}
