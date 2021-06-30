#include "common.h"
#include "sys/dlls/dlls.h"
#include "variables.h"

#pragma GLOBAL_ASM("asm/nonmatchings/object/init_objects.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020778.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020A40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020A48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020BB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020D34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020DA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020EE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800210DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80021150.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80021178.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800211B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800212D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800212E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800212F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800213A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800213EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/objSetupObjectActual.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80021A84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80021CC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80021E74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022150.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022274.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800222AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022338.s")

void copy_obj_position_mirrors(TActor *obj)
{
    DllInstance **dll;
    obj->unk0x44 = obj->ptr0x50[0x2D];
    dll = obj->dll;
    if(1) {
        if(dll != NULL) {
            obj->dll[0]->unk4.withOneArg((s32)obj);
        }
    }

    obj->positionMirror2.x = obj->pos.position.x;
    obj->positionMirror3.x = obj->pos.position.x;
    obj->positionMirror2.y = obj->pos.position.y;
    obj->positionMirror3.y = obj->pos.position.y;
    obj->positionMirror2.z = obj->pos.position.z;
    obj->positionMirror3.z = obj->pos.position.z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/object/some_object_struct_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002272C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800227AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022828.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022868.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800228D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002298C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800229E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022AA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022C68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022D00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022DD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022DEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022DFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022E3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022EC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022F94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800233F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023464.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023628.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023894.s")

TActor *get_player(void) {
    TActor **obj;
    int idx;
    obj = TActor_getter(0, &idx);
    if(idx) {} else {}; //wat
    if(idx) return *obj;
    else return NULL;
}

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002394C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023984.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023994.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800239C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023A00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023A18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023A78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023B34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023B60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023BF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023C6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023CD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023D08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023D30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800240BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024108.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002493C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024D74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024E2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024E50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800250F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025140.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025540.s")
#else
//in retail SFA 1.0 I believe this is at 8002ed18
//but it's different because SFA doesn't use Model Instances
//or, this might be a completely different function...
void func_80025540(TActor *obj, s32 a1, s32 a2)
{
    Model *model = obj->models[obj->curModel];
    void **anims; //this is a struct of some sort.
    if(model->unk66) {
        anims = model->animations;
        if (!anims) { }
        func_800255F8(obj, model, anims, a1, (short) a2); //probably ObjSetBlendMove
        //not certain about the order/count of params here.
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002559C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800255F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025CD4.s")
