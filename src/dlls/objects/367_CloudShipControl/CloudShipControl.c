#include "sys/camera.h"
#include "sys/curves.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/rand.h"
#include "dll.h"

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

typedef struct {
    Object* unk0;
    s32 unk4;
    s32 unk8;
    s8 unkC;
} BSS138;

typedef struct {
    s16 unk0;
    s8 unk2;
    s8 unk3;
    f32 unk4;
} Data10;

/*0x0*/ static s8 data_0 = 1;
/*0x4*/ static s8 data_4 = 0;
/*0x8*/ static s8 data_8 = 0;
/*0xC*/ static Object* data_C = NULL;
/*0x10*/ static Data10 data_10[] = {
    {17, 0, 1, 1.0f},
    {17, 0, 2, 1.0f}, 
    {-1, 0, 0, 0.0f}
};

/*0x0*/ static s32 bss_0;
/*0x4*/ static s32 bss_4;
/*0x8*/ static BSS8 bss_8[11];
/*0x90*/ static BSS90 bss_90[20];
/*0x130*/ static s8 bss_130;
/*0x138*/ static BSS138 bss_138[2];
/*0x158*/ static s8 bss_158;
/*0x160*/ static BSS138 bss_160[2];

static void CloudShipControl_func_728(Object* self);
static void CloudShipControl_func_7F0(Object* self);
static void CloudShipControl_func_9DC(Object* self);
static void CloudShipControl_func_A3C(Object* self);
static void CloudShipControl_func_D64(BSS138*, s32, s8);

// offset: 0x0 | ctor
void CloudShipControl_ctor(void* dll) {
    data_0 = 1;
}

// offset: 0x24 | dtor
void CloudShipControl_dtor(void* dll) { }

// offset: 0x30 | func: 0 | export: 0
void CloudShipControl_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    objInitMesgQueue(self, 10);
}

// offset: 0x74 | func: 1 | export: 1
void CloudShipControl_obj_Control(Object* self) {
    u32 mesgID;
    Object* sender;
    s32 var_a0;
    s32 mesgArg;
    u16 var_a3;
    s32 i;
    s32 k;

    mesgArg = NULL;
    if (data_0 != 0) {
        bss_0 = gDLL_26_Curves->vtbl->func_218C(4);
        bss_4 = gDLL_26_Curves->vtbl->func_218C(2);
        if (bss_0 != -1) {
            CloudShipControl_func_728(self);
        }
        if (bss_4 != -1) {
            CloudShipControl_func_9DC(self);
        }
        data_0 = 0;
    }
    data_4 = 0;
    while (objRecvMesg(self, &mesgID, &sender, (void*)&mesgArg) != 0) {
        switch (mesgID) {
        case 0xF0004:
            if (sender->mapID == self->mapID) {
                var_a0 = 0;
                for (i = 0; i < data_8; i++) {
                    if (sender == bss_90[i].unk0) {
                        bss_90[i].unk4 = mesgArg;
                        var_a0 = 1;
                    }
                }
                if (var_a0 == 0) {
                    bss_90[data_8].unk0 = sender;
                    bss_90[data_8].unk6 = 0;
                    bss_90[data_8].unk4 = mesgArg;
                    data_8++;
                }
                objSendMesg(sender, 0xF0003, self, NULL);
            }
            break;
        case 0xF0007:
            for (i = 0; i < data_8; i++) {
                if (mesgArg == bss_90[i].unk4) {
                    bss_90[i].unk6 = 0;
                    objSendMesg(bss_90[i].unk0, 0xF0007, self, NULL);
                }
            }
            break;
        case 0xF0006:
            for (i = 0; i < data_8; i++) {
                if (mesgArg == bss_90[i].unk4) {
                    bss_90[i].unk6 = 0;
                    objSendMesg(bss_90[i].unk0, 0xF0006, self, NULL);
                }
            }
            break;
        case 0xF0005:
            k = 1;
            for (i = 0; i < data_8; i++) {
                if (sender == bss_90[i].unk0) {
                    bss_90[i].unk6 = 1;
                }
                if ((mesgArg == bss_90[i].unk4) && (bss_90[i].unk6 == 0)) {
                    k = 0;
                }
            }
            if (k != 0) {
                for (i = 0; i < data_8; i++) {
                    if (mesgArg == bss_90[i].unk4) {
                        bss_90[i].unk6 = 0;
                        objSendMesg(bss_90[i].unk0, 0xF0006, self, NULL);
                    }
                }
            }
            break;
        case 0xF0008:
            if (sender == data_C) {
                data_C = NULL;
            } else {
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
            }
            break;
        case 0xF0009:
            data_C = sender;
            break;
        case 0xF000A:
            bss_138[0].unk0 = sender;
            bss_138[0].unk4 = mesgArg;
            bss_138[0].unk8 = -1;
            bss_138[0].unkC = 1;
            objSendMesg(bss_138[0].unk0, 0xF0003, self, NULL);
            break;
        default:
            bss_8[data_4].unk0 = mesgID;
            bss_8[data_4].unk4 = sender;
            bss_8[data_4].unk8 = (Object*)mesgArg;
            data_4++;
            break;
        }
    }
    if (bss_0 != -1) {
        CloudShipControl_func_7F0(self);
    }
    if (bss_4 != -1) {
        CloudShipControl_func_A3C(self);
    }
    if (data_C == NULL) {
        // note: object ID 0xB1 does not exist in this build
        objSendMesgMany(0xB1, OBJMSG_SEND_FILTER_ID, self, 0xF0009, NULL);
    }
}

// offset: 0x654 | func: 2 | export: 2
void CloudShipControl_obj_Update(Object* self) { }

// offset: 0x660 | func: 3 | export: 3
void CloudShipControl_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x6B4 | func: 4 | export: 4
void CloudShipControl_obj_Free(Object* self, s32 onlySelf) {
    objSendMesg(data_C, 0xF0008, self, NULL);
}

// offset: 0x704 | func: 5 | export: 5
u32 CloudShipControl_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x714 | func: 6 | export: 6
u32 CloudShipControl_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x728 | func: 7
static void CloudShipControl_func_728(Object* self) {
    s32 i;

    i = 0;
    while (data_10[i].unk0 >= 0) {
        if (gDLL_26_Curves->vtbl->func_21FC(bss_0, -1, data_10[i].unk3, NULL) != -1) {
            bss_158++;
        }
        i++;
    }
}

// offset: 0x7F0 | func: 8
static void CloudShipControl_func_7F0(Object* self) {
    s32 sp44;
    s32 var_s2;
    BSS138* ptr;

    for (sp44 = 0; sp44 < data_4; sp44++) {
        for (var_s2 = 0; var_s2 < bss_158; var_s2++) {
            if (bss_8[sp44].unk4 == bss_160[var_s2].unk0) {
                ptr = &bss_160[var_s2];
                switch (bss_8[sp44].unk0) {
                case 0xC:
                    ptr->unk0 = bss_8[sp44].unk8;
                    objSendMesg(ptr->unk0, 0xF0003, self, NULL);
                    break;
                case 0xD:
                    ptr->unk0 = bss_8[sp44].unk8;
                    objSendMesg(ptr->unk0, 0xF0003, self, NULL);
                    break;
                case 0xF0001:
                    CloudShipControl_func_D64(ptr, 0, ptr->unkC);
                    objSendMesg(ptr->unk0, 0xF0002, self, (void* ) ptr->unk4);
                    break;
                case 0xE0000:
                    ptr->unk0 = NULL;
                    break;
                }
            }
        }
    }
}

// offset: 0x9DC | func: 9
static void CloudShipControl_func_9DC(Object* self) {
    if (gDLL_26_Curves->vtbl->FindByUID(bss_4) != NULL) {
        bss_130 = 1;
    }
}

// offset: 0xA3C | func: 10
static void CloudShipControl_func_A3C(Object* self) {
    s32 sp84;
    s32 var_s4;
    BSS138* ptr;
    BSS8* ptr2;
    Object* temp_v1;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    CurveSetup* temp_v0_2;

    for (sp84 = 0; sp84 < data_4; sp84++) {
        for (var_s4 = 0; var_s4 < bss_130; var_s4++) {
            ptr2 = &bss_8[sp84];
            ptr = &bss_138[var_s4];
            if (ptr->unk0 == ptr2->unk4) {
                temp_v1 = ptr2->unk4;
                switch (ptr2->unk0) {
                case 0xC:
                    ptr->unk0 = ptr2->unk8;
                    objSendMesg(ptr->unk0, 0xF0003, self, NULL);
                    break;
                case 0xD:
                    ptr->unk0 = ptr2->unk8;
                    objSendMesg(ptr->unk0, 0xF0003, self, NULL);
                    break;
                case 0xF0001:
                    temp_v0_2 = gDLL_26_Curves->vtbl->FindByUID(ptr->unk4);
                    if ((temp_v0_2 != NULL) && (temp_v0_2->curveType == 0x16) && (ptr->unk8 != -1)) {
                        ptr->unk8 = ptr->unk4;
                        if (self->parent != NULL) {
                            camTransformPointByObject(
                                temp_v1->srt.transl.x, temp_v1->srt.transl.y, temp_v1->srt.transl.z, 
                                &sp70, &sp6C, &sp68, 
                                self->parent);
                        } else {
                            sp70 = temp_v1->srt.transl.x;
                            sp6C = temp_v1->srt.transl.y;
                            sp68 = temp_v1->srt.transl.z;
                        }
                        ptr->unk4 = gDLL_26_Curves->vtbl->func_277C(sp70, sp6C, sp68, self->mapID);
                        objSendMesg(ptr->unk0, 0xF0002, self, (void* ) ptr->unk4);
                        objSendMesg(data_C, 0xF000A, ptr->unk0, (void* ) ptr->unk4);
                        ptr->unk0 = NULL;
                    } else {
                        CloudShipControl_func_D64(ptr, 0, ptr->unkC);
                        objSendMesg(ptr->unk0, 0xF0002, self, (void* ) ptr->unk4);
                    }
                    break;
                case 0xE0000:
                    ptr->unk0 = NULL;
                    gDLL_26_Curves->vtbl->FindByUID(bss_4);
                    break;
                }
            }
        }
    }
}

// offset: 0xD64 | func: 11
static void CloudShipControl_func_D64(BSS138* arg0, s32 arg1, s8 arg2) {
    s32 var_a3;
    s32 var_a2;
    CurveSetup* temp_v0_2;
    s32 sp44[4];
    s32 sp40;
    CurveSetup* temp_v0;

    temp_v0 = gDLL_26_Curves->vtbl->FindByUID(arg0->unk4);
    if (temp_v0 == NULL) {
        return;
    }
    if ((arg2 == temp_v0->unk.unk31) || (arg2 == temp_v0->unk.unk32) || (arg2 == temp_v0->unk.unk33)) {
        var_a2 = 0;
        var_a3 = 0;
        while (var_a3 < 5) { // @bug: curves can only have 4 links, not 5
            if (temp_v0->links[var_a3] >= 0) {
                temp_v0_2 = gDLL_26_Curves->vtbl->FindByUID(temp_v0->links[var_a3]);
                if ((arg2 == temp_v0_2->unk.unk31) || (arg2 == temp_v0_2->unk.unk32) || (arg2 == temp_v0_2->unk.unk33)) {
                    sp44[var_a2] = temp_v0->links[var_a3];
                    var_a2 += 1;
                }
            }
            var_a3 += 1;
        }
        if (var_a2 == 0) {
            arg0->unk4 = -1;
        } else if (var_a2 == 1) {
            if ((arg1 == 1) && (arg0->unk8 == sp44[0])) {
                arg0->unk4 = -1;
                arg0->unk8 = -1;
            } else {
                arg0->unk8 = arg0->unk4;
                arg0->unk4 = sp44[0];
            }
        } else if (var_a2 >= 2) {
            var_a3 = 0;
            while (var_a3 < var_a2) {
                if (arg0->unk8 == sp44[var_a3]) {
                    while (var_a3 < var_a2 - 1) {
                        sp44[var_a3] = sp44[var_a3 + 1];
                        var_a3 += 1;
                    }
                    var_a2--;
                }
                var_a3 += 1;
            }
            arg0->unk8 = arg0->unk4;
            arg0->unk4 = sp44[mathRnd(0, var_a2 - 1)];
        }
    } else {
        if ((gDLL_26_Curves->vtbl->func_21FC(arg0->unk4, -1, arg0->unkC, &sp40) != -1) && (sp40 != -1)) {
            arg0->unk8 = arg0->unk4;
            arg0->unk4 = temp_v0->links[sp40];
        }
    }
}
