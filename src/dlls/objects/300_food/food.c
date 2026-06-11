#include "common.h"
#include "sys/objmsg.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
} DLL300_Setup;

typedef struct {
    u8 unk0;
    u8 unk1;
} DLL300_Data;

// offset: 0x0 | ctor
void dll_300_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_300_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_300_setup(Object* self, DLL300_Setup* objSetup, s32 reset) {
    DLL300_Data* objData;

    objData = self->data;

    objData->unk0 = 0;
    self->srt.yaw = get_player()->srt.yaw;

    self->srt.scale = 0.01f;
    self->stateFlags |= 0x4000;
    self->stateFlags |= 0x2000;
    
    switch (self->id) {
    case (OBJ_foodbagNewMeat - 1):
        break;
    case (OBJ_foodbagNewMeat): //0x314
    case (OBJ_foodbagNewFish): //0x315
        break;
    case OBJ_foodbagBlueMush: //0x400
    case OBJ_foodbagBlueGrub: //0x401
    case OBJ_foodbagGreenApp: //0x41A
    case OBJ_foodbagBrownApp: //0x41B
    case OBJ_foodbagSmokedFi: //0x41C
    case OBJ_foodbagOldMeat: //0x41D
    case OBJ_foodbagRedBean: //0x41E
    case OBJ_foodbagBrownBea: //0x41F
    case OBJ_foodbagBlueBean: //0x420
    case OBJ_foodbagRedMushr: //0x421
    case OBJ_foodbagRedGrub: //0x422
    case OBJ_foodbagOldMushr: //0x423
    case OBJ_foodbagOldGrub: //0x424
        break;
    }
    
    obj_init_mesg_queue(self, 1); 
}

// offset: 0xF8 | func: 1 | export: 1
void dll_300_control(Object* self) {
    s32 sp34;
    Object* foodbag;
    Object* player;
    DLL300_Data* objData;

    player = get_player();
    objData = self->data;
    
    if (objData->unk0 == 0) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objData->unk0 = 1;
        return;
    }
    
    if (objData->unk1 != 0) {
        objData->unk1 += gUpdateRateF;
        if (objData->unk1 > 0x80) {
            switch (self->id) {
            case 0x3B:
            case 0x2DB:
            case 0x313:
            case 0x314:                             
            case 0x41A:                             
            case 0x41B:                             
            case 0x41C:                             
            case 0x41D:                             
            case 0x41E:                             
            case 0x41F:                             
            case 0x420:                             
                foodbag = ((DLL_Unknown*)player->dll)->vtbl->func[66].withTwoArgsS32(player, 0xF);
                ((DLL_Unknown*)foodbag->dll)->vtbl->func[9].withTwoArgs(foodbag, self);
                break;
            case 0x400:                         
            case 0x401:                         
            case 0x421:                         
            case 0x422:                         
            case 0x423:                         
            case 0x424:                         
                foodbag = ((DLL_Unknown*)player->dll)->vtbl->func[66].withTwoArgsS32(player, 0x10);
                ((DLL_Unknown*)foodbag->dll)->vtbl->func[9].withTwoArgs(foodbag, self);
                break;
            }
        }
        return;
    }
        
    while (obj_recv_mesg(self, &sp34, 0, 0) != 0) {
        if (sp34 == 0x7000B) {
            switch (self->id) {
            case 0x3B:
            case 0x2DB:
            case 0x313:
            case 0x314:                         
            case 0x41A:                         
            case 0x41B:                         
            case 0x41C:                         
            case 0x41D:                         
            case 0x41E:                         
            case 0x41F:                         
            case 0x420:                         
                foodbag = ((DLL_Unknown*)player->dll)->vtbl->func[66].withTwoArgsS32(player, 0xF);
                switch (self->id) {
                case 0x41A:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 1);
                    break;
                case 0x3B:                      
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 2);
                    break;
                case 0x41B:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 4);
                    break;
                case 0x314:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 8);
                    break;
                case 0x41C:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x10);
                    break;
                case 0x313:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x20);
                    break;
                case 0x41D:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x40);
                    break;
                case 0x2DB:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x80);
                    break;
                case 0x41E:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x100);
                    break;
                case 0x41F:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x200);
                    break;
                case 0x420:                     
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x400);
                    break;
                }
                break;
            case 0x400:                     
            case 0x401:                     
            case 0x421:                     
            case 0x422:                     
            case 0x423:                     
            case 0x424:                     
                foodbag = ((DLL_Unknown*)player->dll)->vtbl->func[66].withTwoArgsS32(player, 0x10);
                switch (self->id) {
                case 0x400:                 
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 1);
                    break;
                case 0x421:                 
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 2);
                    break;
                case 0x423:                 
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 4);
                    break;
                case 0x401:                 
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 8);
                    break;
                case 0x422:                 
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x10);
                    break;
                case 0x424:                 
                    ((DLL_Unknown*)foodbag->dll)->vtbl->func[11].withTwoArgs(foodbag, 0x20);
                    break;
                }
                break;
            }
            objData->unk1 = 1;
            return;
        }
    }

    switch (self->id) {
    case 0x3B:                                  
    case 0x2DB:                                 
    case 0x313:                                 
    case 0x314:                                 
    case 0x400:                                 
    case 0x402:                                 
    case 0x403:                                 
    case 0x404:                                 
    case 0x405:                                 
    case 0x406:                                 
    case 0x407:                                 
    case 0x408:                                 
    case 0x409:                                 
    case 0x40A:                                 
    case 0x40B:                                 
    case 0x40C:                                 
    case 0x40D:                                 
    case 0x40E:                                 
    case 0x40F:                                 
    case 0x410:                                 
    case 0x411:                                 
    case 0x412:                                 
    case 0x413:                                 
    case 0x414:                                 
    case 0x415:                                 
    case 0x416:                                 
    case 0x417:                                 
    case 0x418:                                 
    case 0x419:                                 
    case 0x41A:                                 
    case 0x41B:                                 
    case 0x41C:                                 
    case 0x41D:                                 
    case 0x41E:                                 
    case 0x41F:                                 
    case 0x420:                                 
    case 0x421:                                 
    case 0x423:                                 
        break;
    case 0x401:                                 
    case 0x422:                                 
    case 0x424:                                 
        func_80024108(self, 0.01f, gUpdateRateF, NULL);
        break;
    }
    
    if (self->unkAF & 1) {
        obj_send_mesg(get_player(), 0x7000A, self, 0x95);
    }
}

// offset: 0x82C | func: 2 | export: 2
void dll_300_update(Object *self) { }

// offset: 0x838 | func: 3 | export: 3
void dll_300_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x850 | func: 4 | export: 4
void dll_300_free(Object *self, s32 onlySelf) { }

// offset: 0x860 | func: 5 | export: 5
u32 dll_300_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x870 | func: 6 | export: 6
u32 dll_300_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL300_Data);
}

// offset: 0x884 | func: 7 | export: 7
u8 dll_300_func_884(Object* self, s32 arg1) {
    DLL300_Data* objData = self->data;
    return objData->unk1;
}
