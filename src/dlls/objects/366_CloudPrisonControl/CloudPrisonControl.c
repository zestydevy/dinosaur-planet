#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "dll.h"
#include "macros.h"

typedef struct {
    u32 unk0;
    Object* unk4;
    Object* unk8;
} BSS8;

typedef struct {
    Object* unk0;
    s16 unk4;
    s8 unk6;
} BSS90;

/*0x0*/ static s8 data_0 = 1;
/*0x4*/ static s8 data_4 = 0;
/*0x8*/ static s8 data_8 = 0;

/*0x0*/ static s32 bss_0;
/*0x8*/ static BSS8 bss_8[11];
/*0x90*/ static BSS90 bss_90[20];

static void CloudPrisonControl_func_3C0(Object* self);

// offset: 0x0 | ctor
void CloudPrisonControl_ctor(void* dll) {
    data_0 = 1;
}

// offset: 0x24 | dtor
void CloudPrisonControl_dtor(void* dll) { }

// offset: 0x30 | func: 0 | export: 0
void CloudPrisonControl_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    objInitMesgQueue(self, 10);
}

// offset: 0x74 | func: 1 | export: 1
void CloudPrisonControl_obj_Control(Object* self) {
    u32 mesgID;
    Object* sender;
    s32 var_a1;
    s32 mesgArg;
    s32 i;
    s32 k;

    mesgArg = NULL;
    if (data_0 != 0) {
        bss_0 = gDLL_26_Curves->vtbl->func_218C(8);
        if (bss_0 != -1) {
            CloudPrisonControl_func_3C0(self);
        }
        data_0 = 0;
    }
    data_4 = 0;
    while (objRecvMesg(self, &mesgID, &sender, (void*)&mesgArg) != 0) {
        switch (mesgID) {
        case 0xF0004:
            if (sender->mapID == self->mapID) {
                var_a1 = 0;
                for (i = 0; i < data_8; i++) {
                    if (sender == bss_90[i].unk0) {
                        bss_90[i].unk4 = mesgArg;
                        var_a1 = 1;
                    }
                }
                if (var_a1 == 0) {
                    bss_90[data_8].unk0 = sender;
                    bss_90[data_8].unk6 = 0;
                    bss_90[data_8].unk4 = mesgArg;
                    data_8++;
                }
                objSendMesg(sender, 0xF0003, self, NULL);
            }
            break;
        case 0xF0007:
            STUBBED_PRINTF("Pause\n");
            break;
        case 0xF0006:
            STUBBED_PRINTF("Continue\n");
            break;
        case 0xF0005:
            STUBBED_PRINTF("Level sync\n");
            break;
        case 0xF0008:
            i = 0;
            while (i < data_8 && sender != bss_90[i].unk0) {
                i++;
            }
            data_8 -= 1;
            k = data_8;
            while (k > i) {
                bss_90[k - 1].unk0 = bss_90[k].unk0;
                bss_90[k - 1].unk4 = bss_90[k].unk4;
                bss_90[k - 1].unk6 = bss_90[k].unk6;
                k -= 1;
            }
            break;
        default:
            bss_8[data_4].unk4 = sender;
            bss_8[data_4].unk0 = mesgID;
            bss_8[data_4].unk8 = (Object* )mesgArg;
            data_4 += 1;
            break;
        }
    }
}

// offset: 0x32C | func: 2 | export: 2
void CloudPrisonControl_obj_Update(Object* self) { }

// offset: 0x338 | func: 3 | export: 3
void CloudPrisonControl_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x38C | func: 4 | export: 4
void CloudPrisonControl_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x39C | func: 5 | export: 5
u32 CloudPrisonControl_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x3AC | func: 6 | export: 6
u32 CloudPrisonControl_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x3C0 | func: 7
static void CloudPrisonControl_func_3C0(Object* self) {

}
