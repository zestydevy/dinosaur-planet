//"NWmammoth.c" seems to have been Rare's name!

#include "PR/gbi.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/26_curves.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/227_tumbleweed.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "functions.h"
#include "types.h"
#include "segment_334F0.h"

s32 func_80031BBC(f32, f32, f32);
s32 func_80032538(Object* self);
void joy_set_button_mask(int port, u16 mask);

enum SnowHornAnims {
    MODANIM_SnowHorn_Idle = 0,
    MODANIM_SnowHorn_Talk = 2,
    MODANIM_SnowHorn_Walk = 3,
    MODANIM_SnowHorn_Sleep_Intro = 4,
    MODANIM_SnowHorn_Sleep = 5,
    MODANIM_SnowHorn_Wake_Up = 6,
    MODANIM_SnowHorn_Hit_React = 47
};

enum SnowHornTutorialSequences {
    SEQ_0157_SnowHorn_Chat_BeforeDefeatingSharpClaw = 0, //Hmph, shouldn't you help your friend?
    SEQ_0157_SnowHorn_Chat_BeforeDiggingUpRoot = 1, //I'm too hungry to talk to strangers!
    SEQ_0158_SnowHorn_Chat_AfterDiggingUpRoot = 2, //If you press the C-right button you can feed me that root!
    SEQ_0159_SnowHorn_Cutscene_FeedingRoot1 = 3, //One root is never enough, I'm starving! To get out of the clearing...
    SEQ_0625_SnowHorn_Chat_AfterEatingRoot1 = 4, //One root is never enough, I'm starving!
    SEQ_0248_SnowHorn_Cutscene_FeedingRoot2 = 5, //Ahh, that hit the spot! Tell you what, young 'un, I'll help you out!
    SEQ_0626_SnowHorn_Chat_AfterEatingRoot2 = 6  //I'm sorry lad, you're on your own now.
};

#define GARUNDA_TE_WEEDS_NEEDED 12

typedef struct {
/*000*/ s32 *unk0;
/*004*/ s16 unkRadius;
/*006*/ s16 unk6;
/*008*/ s16 unk8;
/*00A*/ s16 sleepTimer; //randomly-assigned value?
/*00c*/ u16 flags;
/*00e*/ u16 unkE; //yaw?
/*010*/ s32 unk10;
/*014*/ Vec3f playerPositionCopy;
/*020*/ f32 distanceFromPlayer;
/*024*/ s8 unk24;
/*025*/ s8 unk25;
/*026*/ s8 unk26;
/*027*/ s8 unk27;
/*028*/ Object* frostWeed;
/*02c*/ s16 unk2C;
/*02e*/ s16 unk2E;
/*030*/ s32 unk30;
/*034*/ s32 unk34;
/*038*/ s32 unk38;
/*03c*/ s32 unk3C;
/*040*/ s32 unk40;
/*044*/ s16* someAnimIDList;
/*048*/ f32* unk48;
/*04c*/ s32* chatSequenceList;
/*050*/ f32 unk50;
/*054*/ f32 unk54;
/*058*/ f32 walkSpeed; //has something to do with the struct at 0x60?
/*05C*/ s32 unk5C;
/*060*/ UnkCurvesStruct unk60;
/*168*/ s32 unk168;
/*16C*/ s32 unk16C;
/*170*/ DLL27_Data unk170;
/*3d0*/ s8 _unk3D0[0x3E0-0x3D0];
/*3e0*/ u32 unk3e0;
/*3e4*/ u32 unk3e4;
/*3e8*/ u32 unk3e8;
/*3ec*/ u32 unk3ec;
/*3f0*/ u32 unk3f0;
/*3f4*/ u32 unk3f4;
/*3f8*/ u32 unk3f8;
/*3fc*/ u32 unk3fc;
/*400*/ Unk80032CF8 lookAtUnk;
/*424*/ u8 unk424;
/*425*/ u8 unk425;
/*426*/ u8 unk426;
/*427*/ u8 unk427;
/*428*/ s8 garundaTe_weedsEaten;
/*429*/ s8 unk429;
/*42A*/ s8 unk42A;
/*42B*/ s8 unk42B;
// TODO: below is not part of this DLL objdata! where are these supposed to be? does something inherit this DLL?
// /*42C*/ f32 unk42C;
// /*42C*/ s8 unk430[0x16c];
// /*59c*/ s16 rEyeU;
// /*59e*/ s16 rEyeV;
// /*600*/ u32 unk600;
// /*604*/ u32 unk604;
// /*608*/ u32 unk608;
// /*60c*/ s16 lEyeU;
// /*60e*/ s16 lEyeV;
} SnowHorn_Data;

typedef struct{
/*0x10*/ ObjSetup base;
/*0x18*/ s16 unkRadius;
/*0x1A*/ s16 unk1A;
/*0x1C*/ s8 rotation;
/*0x1D*/ s8 unk1D;
} SnowHorn_Setup;

typedef struct {
/*000*/ s8 unk0[0x62];
/*062*/ s8 unk62;
/*063*/ s8 unk63;
/*064*/ s8 unk64[0x2A];
/*08E*/ u8 unk8E[10];
/*098*/ u8 unk98[10];
} UnkStruct2;

static const char _rodata_0[] = "MAM: curve setup failed\n";

static Unk80026DF4 _data_0[] = {
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002f, 0x0030, 0x00,  0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002f, 0x0030, 0x00,  0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002a, 0x0030, 0x00,  0.011, 0.005},
    {SOUND_677_Metal_Clang, NO_SOUND, 0xffff, 0xffff, 0x01,  0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_677_Metal_Clang, NO_SOUND, 0xffff, 0xffff, 0x01,  0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002a, 0x0030, 0x00, 0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002a, 0x0030, 0x00, 0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002a, 0x0030, 0x00, 0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002e, 0x0030, 0x00, 0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002d, 0x0030, 0x00, 0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002c, 0x0030, 0x00, 0.015, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0x002b, 0x0030, 0x00, 0.015, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 0xffff, 0xffff, 0x00, 0.0, 0.0}
};

static Vec3f _data_230[] = {
    { -12, 0, -20 },
    { 12, 0, -20 },
    { 12, 0, 20 },
    { -12, 0, 20 }
};
static f32 _data_260[] = {
    0, 0, 0, 0
};
/** Boolean - decides whether all SnowHorn should go to sleep */
static u8 _data_270 = 0;
static u8 _data_274[] = {0,0,0,0, 0,0,0,0};
static s32 _data_27C[] = {
    3
};
static f32 _data_280[] = {
    0.005, 0.0
};
static s16 _data_288[] = {
    35, 35, 35, 35, 41, 0, 0, 0
};
static f32 _data_298[] = {
    0.005, 0.005, 0.005, 0.005, 0.008, 0.005, 0.005, 0.005
};
static s16 _data_2B8[] = {0};
static f32 _data_2BC[] = {0.005};
static s32 _data_2C0[] = {
    0x0, 0x1
};
static s32 _data_2C8[] = {
    0x2, 0x3
};
static s32 _data_2D0[] = {
    0x4
};
static s32 _data_2D4[] = {
    0x5
};
static s32 _data_2D8[] = {
    0x6
};
static s32 _data_2DC[] = {
    0x7
};
static s32 _data_2E0[] = {
    0x8, 0x9
};
static s32 _data_2E8[] = {
    0x9, 0xb, 0xc, 0xd, 0xe, 0xf
};
static s8 _data_300[] = {
    1,1,1,1
};

void dll_496_ctor(s32 arg0) {
}

void dll_496_dtor(s32 arg0) {
}

//https://decomp.me/scratch/tHX03
//NOTE: dll_496_setup itself matches, it's just that these declared functions need to also be matched in order to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_setup.s")
#else
int dll_496_func_84C(Object* self, Object* overrideObject, AnimObj_Data* animObjdata, s8 arg3);
void dll_496_func_D5C(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_11C4(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_174C(Object *snowHorn, SnowHorn_Data *objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_1CA0(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_22E4(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);

void dll_496_setup(Object* snowhorn, SnowHorn_Setup* mapsObj, s32 arg2) {
    SnowHorn_Data* objdata;
    s32 sp34;

    objdata = snowhorn->data;
    sp34 = *_data_300;
    snowhorn->srt.yaw = mapsObj->rotation << 8;
    snowhorn->animCallback = dll_496_func_84C;
    
    if (arg2 == 0) {
        obj_add_object_type(snowhorn, 0xC);
        objdata->unk50 = 0.005f;
        objdata->unkRadius = mapsObj->unkRadius;
        objdata->unk6 = mapsObj->unk1A * 0x3C;
        snowhorn->unkAF |= 8;

        switch (mapsObj->unk1D) {
            case 0:
                dll_496_func_D5C(snowhorn, objdata, mapsObj);
                break;
            case 2:
                dll_496_func_11C4(snowhorn, objdata, mapsObj);
                break;
            case 1:
            case 3:
                dll_496_func_174C(snowhorn, objdata, mapsObj);
                break;
            case 4:
                dll_496_func_1CA0(snowhorn, objdata, mapsObj);
                break;
            case 5:
                dll_496_func_22E4(snowhorn, objdata, mapsObj);
                break;
        }

        //to do with setting up look-at behaviour?
        if (objdata->unk424 & 1) {
            gDLL_27->vtbl->init(&objdata->unk170, DLL27FLAG_2000000 | DLL27FLAG_4000000, DLL27FLAG_NONE, DLL27MODE_1);
            gDLL_27->vtbl->setup_terrain_collider(&objdata->unk170, 4, _data_230, _data_260, &sp34);
            gDLL_27->vtbl->reset(snowhorn, &objdata->unk170);
        }
        snowhorn->ptr0x64->flags |= 0xA10;
    }
}
#endif

//https://decomp.me/scratch/HgWtR
// referenced funcs just need to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_control.s")
#else
void dll_496_func_11E0(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_1980(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_1D68(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
void dll_496_func_2318(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);
s32 dll_496_func_980(Object* snowhorn);
void dll_496_func_CC4(Object *snowHorn, s32 lookAt);
void dll_496_func_D80(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj);

void dll_496_control(Object* snowhorn) {
    SnowHorn_Data* objdata;
    f32 daytime;
    SnowHorn_Setup* mapsObj;
    Object* player;
    UnkFunc_80024108Struct sp44;
    s32 animIndex;
    s32 seqIndex;

    objdata = snowhorn->data;
    mapsObj = (SnowHorn_Setup*)snowhorn->setup;
    player = get_player();

    if (vec3_distance_xz_squared(&snowhorn->positionMirror, &player->positionMirror) 
            < 2.0f * (objdata->unkRadius * objdata->unkRadius)) {
        if (!(objdata->unk424 & 0x80)) {
            objdata->unk424 |= 0x80;
        }
    } else {
        if (objdata->unk424 & 0x80) {
            objdata->unk424 &= 0xFF7F;
        }
    }
    
   if (objdata->unk424 & 0x40) {
        dll_496_func_CC4(snowhorn, objdata->unk424 & 4);
        func_800328F0(snowhorn, &objdata->lookAtUnk, objdata->walkSpeed);
    }
    func_80032A08(snowhorn, &objdata->lookAtUnk);

    if (func_80026DF4(snowhorn, _data_0, 0x1C, (objdata->flags & 0x4000 ? 1 : 0), 
                      &objdata->unk54) != 0) {
        objdata->flags |= 0x4000;
        return;
    }
    
    objdata->flags &= 0xBFFF;
    objdata->unk427 = gDLL_29_Gplay->vtbl->get_map_setup(snowhorn->mapID);
    _data_270 = gDLL_7_Newday->vtbl->func8(&daytime); //check if night

    if (objdata->flags & 0x8000 && (dll_496_func_980(snowhorn) != 0)){
        return;
    }

    objdata->distanceFromPlayer = vec3_distance(&snowhorn->positionMirror, &player->positionMirror);
    switch (mapsObj->unk1D) {
        case 0:
            dll_496_func_D80(snowhorn, objdata, mapsObj);
            break;
        case 1:
        case 2:
            dll_496_func_11E0(snowhorn, objdata, mapsObj);
            break;
        case 3:
            dll_496_func_1980(snowhorn, objdata, mapsObj);
            break;
        case 4:
            dll_496_func_1D68(snowhorn, objdata, mapsObj);
            break;
        case 5:
            dll_496_func_2318(snowhorn, objdata, mapsObj);
            break;
    }
    
    if (objdata->unk424 & 1) {
        gDLL_27->vtbl->func_1E8(snowhorn, &objdata->unk170, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(snowhorn, &objdata->unk170);
        gDLL_27->vtbl->func_624(snowhorn, &objdata->unk170, gUpdateRateF);
    }

    if (objdata->someAnimIDList) {
        animIndex = objdata->flags & 0xFFFF7FFF;
        if (snowhorn->curModAnimId != objdata->someAnimIDList[animIndex]) {
            func_80023D30(snowhorn, objdata->someAnimIDList[animIndex], 0.0f, 0);

            if (objdata->unk48[animIndex] >= 0.0f) {
                objdata->unk50 = objdata->unk48[animIndex];
            }
            objdata->unk424 &= 0xFFF7;
        }
        if (func_80024108(snowhorn, objdata->unk50, gUpdateRateF, &sp44) != 0) {
            objdata->unk424 |= 8;
        } else {
            objdata->unk424 &= 0xFFF7;
        }
        func_80025780(snowhorn, gUpdateRateF, &sp44, 0);
    }

    if ((objdata->chatSequenceList != 0) && (snowhorn->unkAF & 1)) {
        if (objdata->unk424 & 0x20) {
            seqIndex = rand_next(0, objdata->unk426 - 1);
        } else {
            seqIndex = objdata->unk425;
        }
        objdata->unk425++;
        if (objdata->unk425 >= objdata->unk426) {
            objdata->unk425 = 0;
        }
        gDLL_3_Animation->vtbl->func17(objdata->chatSequenceList[seqIndex], snowhorn, -1);
        joy_set_button_mask(0, A_BUTTON);
    }

}
#endif

void dll_496_update(u32 a0){
}

s32 func_80031F6C(Object*, s32, f32*, f32*, f32*, s32);

void dll_496_print(Object* self, Gfx **gfx, Mtx **mtx, Vertex **vtx, Triangle **pols, s8 visibility) {
    SnowHorn_Data* objdata;
    u32 addr;

    objdata = self->data;
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
        func_80031F6C(self, 1, &objdata->playerPositionCopy.x, &objdata->playerPositionCopy.y, &objdata->playerPositionCopy.z, 0);
    }
}

void dll_496_free(s32 arg0, s32 arg1) {
    _data_274[0] = 0;
}

u32 dll_496_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS | MODFLAGS_80;
}

u32 dll_496_get_data_size(Object *self, s32 arg1) {
    return sizeof(SnowHorn_Data);
}

/** When talking with SnowHorn */
int dll_496_func_84C(Object* self, Object* overrideObject, AnimObj_Data* animObjdata, s8 arg3) {
    SnowHorn_Data* objdata;
    s32 i;

    objdata = self->data;
    if (arg3 != 0) {
        func_80024108(self, 0.005f, gUpdateRateF, NULL);
    }
    if (objdata->unk424 & 1) {
        gDLL_27->vtbl->reset(self, &objdata->unk170);
    }
    self->unkAF |= 8;
    animObjdata->unk62 = 0;

    for (i = 0; i < animObjdata->unk98; i++){
        if (animObjdata->unk8E[i] == 3) {
            main_set_bits(BIT_5A0, 1);
            continue;
        }        
    }
    
    objdata->walkSpeed = 0.0f;
    return 0;
}

/** SnowHorn sleep state machine: handles anims and sounds */
s32 dll_496_func_980(Object* snowhorn) {
    UnkFunc_80024108Struct sp4c;
    SnowHorn_Data* objdata;
    s32* temp1;
    s32* temp2;
    s32 animIsFinished;
    s32 playSound; //toggles between 0 and 1 (when ready to play sound another time)

    objdata = (SnowHorn_Data*)snowhorn->data;
    animIsFinished = func_80024108(snowhorn, 0.006f, gUpdateRateF, &sp4c);
    
    if (sp4c.unk1B != 0) {
        playSound = sp4c.unk13[0] == 0;
    } else {
        playSound = 0;
    }
    
    temp1 = func_800348A0(snowhorn, MODANIM_SnowHorn_Sleep, 0);
    temp2 = func_800348A0(snowhorn, MODANIM_SnowHorn_Sleep_Intro, 0);
    snowhorn->unkAF |= 8;    
    
    switch (snowhorn->curModAnimId) {
        case MODANIM_SnowHorn_Sleep_Intro:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->play_sound(snowhorn, SFX_129_SnowHorn_Yawn_1, MAX_VOLUME, 0, 0, 0, 0);
            }
            if (animIsFinished) {
                func_80023D30(snowhorn, MODANIM_SnowHorn_Sleep, 0.0f, 0); //play next animation
                if (temp1 != NULL) {
                    *temp1 = 0x200;
                }
                if (temp2 != NULL) {
                    *temp2 = 0x200;
                }
                objdata->sleepTimer = rand_next(0, 300);
            }
            break;
        case MODANIM_SnowHorn_Sleep:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->play_sound(snowhorn, SFX_12A_SnowHorn_SnoreHorn, MAX_VOLUME, 0, 0, 0, 0);
            }
            objdata->sleepTimer-= gUpdateRate;
            if ((_data_270 == 0) && objdata->sleepTimer <= 0) {  //if daytime rolls around
                func_80023D30(snowhorn, MODANIM_SnowHorn_Wake_Up, 0.0f, 0); //play wake-up animation
                if (temp1 != NULL) {
                    *temp1 = 0;
                }
                if (temp2 != NULL) {
                    *temp2 = 0;
                }
            }
            break;
        case MODANIM_SnowHorn_Wake_Up:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->play_sound(snowhorn, SFX_12B_SnowHorn_Yawn_2, MAX_VOLUME, 0, 0, 0, 0);
            }
            if (animIsFinished) {
                func_80023D30(snowhorn, MODANIM_SnowHorn_Idle, 0.0f, 0); //Play idle animation
                objdata->flags &= 0x7FFF;
                snowhorn->unkAF &= 0xFFF7;
                return 0;
            }
            break;
        default:
            objdata->sleepTimer -= gUpdateRate;
            if (objdata->sleepTimer <= 0) { //Go back to sleep if interrupted (by flinch anim etc.)
                func_80023D30(snowhorn, MODANIM_SnowHorn_Sleep_Intro, 0.0f, 0);
                objdata->walkSpeed = 0.0f;
            }
            break;
    }
    return 1;
}

/** Updates the SnowHorn's player position reference when nearby (for the look-at behaviour) */
void dll_496_func_CC4(Object *snowHorn, s32 lookAt){
    SnowHorn_Data *objdata;
    Object *player;
      
    objdata = snowHorn->data;
    player = get_player();
      
    if (lookAt && player && objdata->distanceFromPlayer < 200.0f){
      objdata->lookAtUnk.unk0 = 1;
      objdata->lookAtUnk.x = player->srt.transl.x;
      objdata->lookAtUnk.y = player->srt.transl.y;
      objdata->lookAtUnk.z = player->srt.transl.z;
      return;
    }
  
    objdata->lookAtUnk.unk0 = 0;
}

void dll_496_func_D5C(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj) {
    objdata->flags = 0;
    objdata->unk424 |= 0x44;
    objdata->unkRadius = mapsObj->unkRadius;
}

/** Called by the standing SnowHorn (not by the ones that walk around) */
void dll_496_func_D80(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj) {
    Object* player;

    if (_data_270 != 0) {
        objdata->sleepTimer = rand_next(0, 300);
        objdata->flags |= 0x8000;

        snowhorn->unkAF |= 8;
        snowhorn->unkAF &= -2;
        return;
    }
    
    objdata->flags &= 0x7FFF;
    if (snowhorn->curModAnimId != 0) {
        func_80023D30(snowhorn, 0, 0.0f, 0);
    }

    func_80024108(snowhorn, objdata->unk50, gUpdateRate, NULL);
    player = get_player();
    if (!player) 
        return;
    
    if ((f32)objdata->unkRadius*objdata->unkRadius < vec3_distance_squared(&snowhorn->positionMirror, &player->positionMirror)) {
        objdata->sleepTimer += gUpdateRate;
        if (objdata->sleepTimer > 900) {
            gDLL_3_Animation->vtbl->func17(7, snowhorn, -1); //play seq 7?
            objdata->sleepTimer = (s16) -rand_next(0, 50);
        }
        return;
    } 
    
    objdata->sleepTimer = 0;
    snowhorn->unkAF &= 0xFFF7;

    switch (objdata->flags) {
        u32 rootsEaten;
        case 0:
            if (main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
                objdata->flags = 1;
            }
            break;    
        case 1:
            rootsEaten = main_get_bits(BIT_SnowHorn_Tutorial_NumRootsFed);
            switch (rootsEaten) {
                case 0:
                    if (main_get_bits(BIT_SnowHorn_Tutorial_GotAlpineRoot1) || 
                        main_get_bits(BIT_SnowHorn_Tutorial_GotAlpineRoot2))
                    {
                        objdata->flags = 2;
                    }
                    break;
                case 1:
                    objdata->flags = 4;
                    break;
                default:
                    objdata->flags = 6;
                    break;
            }
            break;
        case 2:
            if ((snowhorn->unkAF & 4) && gDLL_1_UI->vtbl->func_DF4(BIT_SW_Alpine_Roots)) {
                main_set_bits(BIT_SnowHorn_Tutorial_NumRootsFed, 1);
                main_decrement_bits(BIT_SW_Alpine_Roots);
                gDLL_3_Animation->vtbl->func17(SEQ_0159_SnowHorn_Cutscene_FeedingRoot1, snowhorn, -1);
                objdata->flags = 4;
                return;
            }
            break;
        case 4:
            if ((snowhorn->unkAF & 4) && gDLL_1_UI->vtbl->func_DF4(BIT_SW_Alpine_Roots)) {
                main_set_bits(BIT_SnowHorn_Tutorial_NumRootsFed, 2);
                main_decrement_bits(BIT_SW_Alpine_Roots);
                gDLL_3_Animation->vtbl->func17(SEQ_0248_SnowHorn_Cutscene_FeedingRoot2, snowhorn, -1);
                objdata->flags = 6;
                return;
            }
            break;
        case 6:
            break;
    }
    
    //Play chat sequence
    if (snowhorn->unkAF & 1) {
        snowhorn->unkAF &= 0xFFFE;
        if (objdata->flags < 7) {
            gDLL_3_Animation->vtbl->func17(objdata->flags, snowhorn, -1);
            joy_set_button_mask(0, A_BUTTON);
        }
    }

}

void dll_496_func_11C4(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj) {
    objdata->unk424 |= 0x44;
}

//https://decomp.me/scratch/KhEPZ
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_11E0.s")
#else
void dll_496_func_11E0(Object* self, SnowHorn_Data* snowHornObjdata, SnowHorn_Setup* setup) {
    SnowHorn_Data* objdata;
    u32 questValue;
    u16 seqBoneAngle;
    SRT srt;
    Vec3f v;
    s32 pad[3];
    Object* player;
    // Vec3f* position;
    u8 temp_t3;
    s32 var_v1;
    s32 temp_v0;
    s16* temp_v0_2;

    // object = self;
    objdata = self->data;
    
    temp_v0 = func_80024108(self, 0.005f, gUpdateRate, NULL);
    questValue = objdata->flags;

    // position = &objdata->playerPositionCopy;
    switch (questValue) {
        case 0:
            v.x = 0.0f;
            v.y = 10.0f;
            v.z = -25.0f;
            srt.transl.z = 0.0f;
            srt.transl.y = 0.0f;
            srt.transl.x = 0.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = objdata->unkE;
            srt.scale = 0.0f;
            rotate_vec3(&srt, v.f);
            srt.transl.x = v.x + objdata->playerPositionCopy.x;
            srt.transl.y = v.y + self->srt.transl.y;
            srt.transl.z = v.z + objdata->playerPositionCopy.z;
            srt.yaw = 0;
            objdata->flags = 1;
            break;
        case 1:
            if (self->animProgress > 0.25f) {
                v.x = 0.0f;
                v.y = 20.0f;
                v.z = -20.0f;
                srt.transl.z = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.x = 0.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.yaw = objdata->unkE;
                srt.scale = 0.0f;
                rotate_vec3(&srt, v.f);
                srt.transl.x = v.x + objdata->playerPositionCopy.x;
                srt.transl.y = v.y + self->srt.transl.y;
                srt.transl.z = v.z + objdata->playerPositionCopy.z;
                srt.yaw = 0;
                objdata->flags = 2;
            }
            break;
        case 2:
            if (self->animProgress > 0.65f) {
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = -40.0f;
                srt.transl.z = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.x = 0.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.yaw = objdata->unkE;
                srt.scale = 0.0f;
                rotate_vec3(&srt, v.f);
                self->positionMirror.x = objdata->unk10 + 40.0f;
                self->positionMirror.z = self->srt.transl.z;
                self->positionMirror.y = objdata->playerPositionCopy.x;

                srt.yaw = 0;
                self->positionMirror.x = self->positionMirror.x;                
                self->positionMirror.y = self->positionMirror.y;
                self->positionMirror.z = self->positionMirror.z;
                objdata->flags = 3;
            }
            break;
        case 3:
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = -60.0f;
            srt.transl.z = 0.0f;
            srt.transl.y = 0.0f;
            srt.transl.x = 0.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = objdata->unkE;
            srt.scale = 0.0f;
            rotate_vec3(&srt, v.f);
            srt.transl.x = v.x + objdata->playerPositionCopy.x;
            srt.transl.y = v.y + self->srt.transl.y;
            srt.transl.z = v.z + objdata->playerPositionCopy.z;
            srt.yaw = 0;
            objdata->flags = 4;
            break;
        case 4:
            if (temp_v0 != 0) {
                func_80023D30(self, 0, 0.0f, 0);
                objdata->flags = 5;
                return;
            }
            break;
        case 5:
            questValue |= 0x8000;
            if (_data_270 != 0) {
                objdata->flags = questValue;
                temp_t3 = self->unkAF | 8;
                self->unkAF = temp_t3;
                self->unkAF = temp_t3 & 0xFFFE;
                break;
            }
            objdata->unk8 = (s16) (objdata->unk8 + gUpdateRate);
            if (objdata->unk6 < objdata->unk8) {
                objdata->unk8 = 0;
                temp_v0_2 = func_80034804(self, 0);

                objdata->unkE = (u16) (0x8000 - *&temp_v0_2[1]);
                player = get_player();
                
                var_v1 = (arctan2_f((player->positionMirror.x + (player->speed.x * 60.0f)) - self->positionMirror.x, (player->positionMirror.z + (player->speed.z * 60.0f)) - self->positionMirror.z) - (self->srt.yaw & 0xFFFF)) + 0x8000;
                CIRCLE_WRAP(var_v1)
                
                func_80023D30(self, 1, 0.0f, 0);
                if ((var_v1 >= -0xBB7) && (var_v1 < 0xBB8)) {
                    objdata->unk38 = player->srt.transl.x;
                    objdata->unk3C = player->srt.transl.y;
                    objdata->unk40 = player->srt.transl.z;
                    objdata->unk2C = objdata->unkE;
                } else {
                    seqBoneAngle = self->srt.yaw + *&temp_v0_2[1];
                    objdata->unk38 = self->srt.transl.x - (fsin16_precise(seqBoneAngle) * 250.0f);
                    objdata->unk3C = self->srt.transl.y;
                    objdata->unk40 = self->srt.transl.z - (fsin16_precise(seqBoneAngle) * 250.0f);
                    objdata->unk2C = 0;
                }
                objdata->flags = 0;
            }
            break;
    }
}
#endif

//https://decomp.me/scratch/fe0rJ
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_174C.s")
#else
void dll_496_func_174C(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* setup){
    s32 sp2C = 0x19;
    s32 result;
    
    objdata->unk424 |= 0x45;
    objdata->someAnimIDList = (s16*)_data_27C;
    objdata->unk48 = _data_280;

    result = gDLL_26_Curves->vtbl->func_4288(&objdata->unk60, snowhorn, 1000.0f, &sp2C, -1);
    if (!result){
        snowhorn->srt.transl.x = objdata->unk60.unk68;
        snowhorn->srt.transl.z = objdata->unk60.unk70;
        
        objdata->flags = 1;
        objdata->walkSpeed = 0.5f;
    } else {
        objdata->flags = 0;
    }

    if (setup->unk1D == result){
        if (main_get_bits(BIT_SpellStone_DIM_Activated)){
            objdata->chatSequenceList = _data_2E8;
            objdata->unk426 = 2.0f;
        } else if (main_get_bits(BIT_SpellStone_DIM)){
            objdata->chatSequenceList = _data_2DC;
            objdata->unk426 = 1.0f;
        } else if (main_get_bits(BIT_Tricky_Distract_Learned)){
            objdata->chatSequenceList = _data_2D4;
            objdata->unk426 = 1.0f;
        } else {
            objdata->chatSequenceList = _data_2C8;
            objdata->unk426 = 2.0f;
        }
    } else {
        if (main_get_bits(BIT_SpellStone_DIM_Activated)){
            objdata->chatSequenceList = _data_2E0;
            objdata->unk426 = 2.0f;
        } else if (main_get_bits(BIT_SpellStone_DIM)){
            objdata->chatSequenceList = _data_2D8;
            objdata->unk426 = 1.0f;
        } else if (main_get_bits(BIT_Tricky_Distract_Learned)){
            objdata->chatSequenceList = _data_2D0;
            objdata->unk426 = 1.0f;
        } else {
            objdata->chatSequenceList = _data_2C0;
            objdata->unk426 = 2.0f;
        }
    }
}
#endif

void dll_496_func_1980(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* arg2) {
    f32 temp_walkSpeed;
    f32 dx;
    f32 dz;
    f32 speed;
    UnkCurvesStruct* curveStruct;

    if (_data_270) {
        temp_walkSpeed = objdata->walkSpeed;
        if (objdata->walkSpeed > 0.0f) {
            objdata->walkSpeed = temp_walkSpeed - 0.025f;
        } else {
            objdata->flags |= 0x8000;
            objdata->walkSpeed = 0.0f;
            objdata->sleepTimer = rand_next(0, 300);
            return;
        }
    } else if (snowhorn->unkAF & 4 || objdata->distanceFromPlayer < 80.0f) {
        objdata->walkSpeed -= 0.025f;
        if (objdata->walkSpeed < 0.05f) {
            objdata->walkSpeed = 0.0f;
        }
    } else {
        objdata->walkSpeed += 0.025f;
        if (objdata->walkSpeed > 0.5f) {
            objdata->walkSpeed = 0.5f;
        }
    }
    
    if (_data_274[0] != 0 && 
        func_80031BBC(snowhorn->positionMirror.x, snowhorn->positionMirror.y, snowhorn->positionMirror.z) == 0xA){
        gDLL_3_Animation->vtbl->func17(0x10, snowhorn, -1); //setAnimation?
        return;
    }
    snowhorn->unkAF &= 0xFFF7;

    if (objdata->flags != 0) {
        curveStruct = &objdata->unk60;
        if (objdata->flags != 1) {
            objdata->flags = 0;
            return;
        }
        if ((func_800053B0(curveStruct, objdata->walkSpeed) != 0) || (curveStruct->unk10 != 0)) {
            gDLL_26_Curves->vtbl->func_4704(curveStruct);
        }
        dx = curveStruct->unk68 - snowhorn->srt.transl.x;
        dz = curveStruct->unk70 - snowhorn->srt.transl.z;

        //a1 for func_8002493C seems to be speed (obtained by dividing magnitude of dPos by dt)!
        speed = sqrtf((dx * dx) + (dz * dz)) * gUpdateRateInverseF;
        func_8002493C(snowhorn, speed, &objdata->unk50);
        snowhorn->srt.yaw = arctan2_f(curveStruct->unk74, curveStruct->unk7C) + 0x8000;
        snowhorn->srt.transl.x = curveStruct->unk68;
        snowhorn->srt.transl.z = curveStruct->unk70;
        objdata->unk424 &= 0xFFFB;
        
        if (objdata->walkSpeed <= 0.0f) {
            objdata->flags = 0;
        }
    } else {
        objdata->unk424 = (u8) (objdata->unk424 | 4);
        if (0.1f < objdata->walkSpeed) {
            objdata->flags = 1;
        }
    }
}

/** snowHorn_handleGarundaTeFlags? */
void dll_496_func_1CA0(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj) {
    u16 questValue;
    u16 questEnd = 6;

    questValue = (objdata->flags = main_get_bits(BIT_Garunda_Te_Quest_Progress));
    
    /** If the 1st SpellStone is activated, makes sure Garunda Te's flags are in their end state */
    if (questValue < questEnd && main_get_bits(BIT_SpellStone_DIM)){
        objdata->flags = questEnd;
        questValue = objdata->flags;
        main_set_bits(BIT_Garunda_Te_Quest_Progress, questValue);
    }
    
    objdata->someAnimIDList = _data_288;
    objdata->unk48 = _data_298;
    objdata->garundaTe_weedsEaten = main_get_bits(BIT_Garunda_Te_Weeds_Eaten);
}

void dll_496_func_1D68(Object* self, SnowHorn_Data* objdata, SnowHorn_Setup* setup) {
    Object* frostWeed;
    s32 weeds;
    
    self->unkAF &= 0xFFF7;
    switch (objdata->flags) {
        case 0:
            //Calling out to the player periodically
            objdata->unk8 += gUpdateRate;
            if (objdata->unk8 >= 0x3E9) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_1E2_Garunda_Te_Will_somebody_get_me_out_of_here, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_22_Subtitles->vtbl->func_368(0xA);
                objdata->unk8 = 0;
            }
            if (self->unkAF & 4) {
                objdata->flags = 1;
            }
            break;
        case 1:
            if (func_80032538(self)) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                objdata->flags = 2;
                main_set_bits(BIT_Garunda_Te_Quest_Progress, objdata->flags);
            }
            break;
        case 2:
            //Eating FrostWeeds?
            if (func_80032538(self)) {
                gDLL_3_Animation->vtbl->func17(1, self, -1);
            }
            
            frostWeed = obj_get_nearest_type_to(4, self, 0);
            setup = (SnowHorn_Setup*)self->setup;
            if (frostWeed && frostWeed->id == 0x3FB && vec3_distance_xz_squared(&self->positionMirror, &frostWeed->positionMirror) < setup->unkRadius * setup->unkRadius) {
                if (!((DLL_227_Tumbleweed*)frostWeed->dll)->vtbl->func4(frostWeed)) {
                    ((DLL_227_Tumbleweed*)(frostWeed->dll))->vtbl->func3(frostWeed, &objdata->playerPositionCopy);
                    objdata->frostWeed = frostWeed;
                    if (0){
                        objdata->garundaTe_weedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                    }
                    objdata->garundaTe_weedsEaten++;
                    if (objdata->garundaTe_weedsEaten > GARUNDA_TE_WEEDS_NEEDED) {
                        objdata->garundaTe_weedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                    }
                    main_set_bits(BIT_Garunda_Te_Weeds_Eaten, objdata->garundaTe_weedsEaten);
                    objdata->flags = 3;
                }
            }
            break;
        case 3:
            if (vec3_distance_xz_squared(&objdata->playerPositionCopy, &objdata->frostWeed->positionMirror) < 6.25f) {
                objdata->flags = 4;
            }
            break;
        case 4:
            if (objdata->unk424 & 8) {
                weeds = objdata->garundaTe_weedsEaten;
                if (weeds >= GARUNDA_TE_WEEDS_NEEDED) {
                    main_set_bits(BIT_Garunda_Te_Fed, 1);
                    objdata->flags = 5;
                    main_set_bits(BIT_Garunda_Te_Quest_Progress, objdata->flags);
                    break;
                }
                if (weeds % 3 == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_74B_Garunda_Te_That_tastes_great_Hurry_up_boy, MAX_VOLUME, 0, 0, 0, 0);
                    gDLL_22_Subtitles->vtbl->func_368(2);
                }
                objdata->flags = 2;
            }
            break;
        case 5:
            if (func_80032538(self)) {
                if (objdata->unk425 % 2) {
                    gDLL_3_Animation->vtbl->func17(3, self, -1);
                } else {
                    gDLL_3_Animation->vtbl->func17(2, self, -1);
                }
                objdata->unk425 += 1;
            }
            break;
        case 6:
            //SpellStone activation
            if (func_80032538(self)) {
                gDLL_3_Animation->vtbl->func17(4, self, -1);
            } else if (gDLL_1_UI->vtbl->func_DF4(0x123)) {
                main_set_bits(BIT_SpellStone_DIM_Activated, 1);
                objdata->flags = 7;
                main_set_bits(BIT_Garunda_Te_Quest_Progress, objdata->flags);
            }
            break;
        case 7:
            self->unkAF |= 8;
            break;
    }
    
    if (objdata->flags >= 2 && objdata->flags < 5) {
        diPrintf("noweeds=%d\n", objdata->garundaTe_weedsEaten);
    }
}

void dll_496_func_22E4(Object *snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj) {
    objdata->someAnimIDList = _data_2B8;
    objdata->unk48 = _data_2BC;
}

void dll_496_func_2318(Object* snowhorn, SnowHorn_Data* objdata, SnowHorn_Setup* mapsObj) {
}
